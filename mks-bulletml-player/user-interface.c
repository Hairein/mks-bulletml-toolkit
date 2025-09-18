#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "user-interface.h"

void init_user_interface(UserInterface* ui, char xml_filenames[MKSBMLI_MAX_PLAYBACK_HANDLES][MKSBMLI_XML_FILENAME_MAX_LENGTH], int xml_count,
    int v_width, int v_height) {
    memset(ui, 0, sizeof(*ui));

    ui->nos_loaded_xmls_files = xml_count;

    copy_xml_filenames(ui, xml_filenames);

    ui->stop_button_texture = LoadTexture("../assets/textures/button_stop.png");
    ui->play_button_texture = LoadTexture("../assets/textures/button_play.png");
    ui->play_frame_button_texture = LoadTexture("../assets/textures/button_play_frame.png");
    ui->pause_button_texture = LoadTexture("../assets/textures/button_pause.png");

    ui->stop_requested = false;
    ui->play_requested = false;
    ui->play_frame_requested = false;
    ui->pause_requested = false;

    snprintf(ui->width_buffer, MKSBMLP_NUMBER_TEXT_WIDTH, "%d", v_width);
    ui->width_buffer_edit = false;
    snprintf(ui->height_buffer, MKSBMLP_NUMBER_TEXT_WIDTH, "%d", v_height);
    ui->height_buffer_edit = false;
    ui->change_virtual_dims_requested = false;

    prepare_displayed_file_list(ui);
    build_displayed_file_list(ui);
}

void render_user_interface(UserInterface* ui) {
    if (ui->nos_loaded_xmls_files > 0) {
        if (GuiDropdownBox((Rectangle){ 0, 0, 320, 30 }, ui->displayed_xml_file_list, &ui->active_xml_file, ui->active_xml_file_edit_mode))
        {
            ui->active_xml_file_edit_mode = !ui->active_xml_file_edit_mode;

            if(!ui->active_xml_file_edit_mode) {
                ui->active_xml_file_change_requested = true;
            }
        }

        char page_display[MKSBMLP_MAX_LINE_LENGTH];
        snprintf(page_display, MKSBMLP_MAX_LINE_LENGTH, "%d/%d", ui->current_page + 1, ui->nos_pages + 1);
        GuiLabel((Rectangle){ 322, 0, 146, 30 }, page_display);

        if(GuiButton((Rectangle){ 360, 0, 48, 30 }, "-")) previous_page(ui);
        if(GuiButton((Rectangle){ 412, 0, 48, 30 }, "+")) next_page(ui);

        int panel_start_y = GetScreenHeight() - (int)TOP_BOTTOM_CUTOFF_HALF;

        if(CustomGuiImageButton((Rectangle){ 0, panel_start_y, 30, 30 }, ui->stop_button_texture)) ui->stop_requested |= true;
        if(CustomGuiImageButton((Rectangle){ 30, panel_start_y, 30, 30 }, ui->play_button_texture)) ui->play_requested |= true;
        if(CustomGuiImageButton((Rectangle){ 60, panel_start_y, 30, 30 }, ui->play_frame_button_texture)) ui->play_frame_requested |= true;
        if(CustomGuiImageButton((Rectangle){ 90, panel_start_y, 30, 30 }, ui->pause_button_texture)) ui->pause_requested |= true;
        GuiLabel((Rectangle){ 120, panel_start_y, 96, 30 }, ui->play_state_buffer);

        GuiLabel((Rectangle){ 264, panel_start_y, 146, 30 }, "Playfield Dimensions (WxHpx):");

        Rectangle width_rect = (Rectangle){ 418, panel_start_y, 48, 30 };
        if(!ui->width_buffer_edit && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), width_rect)) ui->width_buffer_edit = true;
        if(GuiTextBox(width_rect, ui->width_buffer, 8, ui->width_buffer_edit)) validate_width(ui);

        Rectangle height_rect = (Rectangle){ 466, panel_start_y, 48, 30 };
        if(!ui->height_buffer_edit && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), height_rect)) ui->height_buffer_edit = true;
        if(GuiTextBox(height_rect,  ui->height_buffer, 8, ui->height_buffer_edit)) validate_height(ui) ;

        if(GuiButton((Rectangle){ 514, panel_start_y, 48, 30 }, "Set")) update_dims(ui);
    }
}

void previous_page(UserInterface* ui) {
    ui->current_page--;
    if(ui->current_page < 0) ui->current_page = 0;

    build_displayed_file_list(ui);
}

void next_page(UserInterface* ui) {
    ui->current_page++;
    if(ui->current_page > ui->nos_pages) ui->current_page = ui->nos_pages;

    build_displayed_file_list(ui);
}

void prepare_displayed_file_list(UserInterface* ui) {
    ui->current_page = 0;
    ui->nos_pages = ui->nos_loaded_xmls_files / MKSBMLP_FILES_PER_PAGE;

    ui->active_xml_file = 0;
    ui->active_xml_file_edit_mode = false;
}

void build_displayed_file_list(UserInterface* ui) {
    memset(ui->displayed_xml_file_list, 0, sizeof(char) * MKSBMLP_MAX_LINE_LENGTH);

    int page_start = ui->current_page * MKSBMLP_FILES_PER_PAGE;
    int page_end = page_start + MKSBMLP_FILES_PER_PAGE;
    for(int index = page_start; index < page_end; index++) {
        if (ui->displayed_xml_file_list[0] != '\0') {
            strncat(ui->displayed_xml_file_list, ";", MKSBMLP_MAX_LINE_LENGTH - strlen(ui->displayed_xml_file_list) - 1);
        }

        strncat(ui->displayed_xml_file_list, ui->xml_filenames_copy[index], MKSBMLP_MAX_LINE_LENGTH - strlen(ui->displayed_xml_file_list) - 1);
    }

    ui->active_xml_file = 0;
}

void copy_xml_filenames(UserInterface* ui, char xml_filenames[MKSBMLI_MAX_PLAYBACK_HANDLES][MKSBMLI_XML_FILENAME_MAX_LENGTH]) {
    for(int index = 0; index < ui->nos_loaded_xmls_files; index++) {
        memset(ui->xml_filenames_copy[index], 0, MKSBMLP_MAX_LINE_LENGTH);

        const char* filename = xml_filenames[index];

        if (filename[0] == '\0') continue;

        const char* shortname = strrchr(filename, '/');
        if (!shortname) shortname = strrchr(filename, '\\');
        if (shortname) shortname++; else shortname = filename;

        strncpy(ui->xml_filenames_copy[index], shortname, MKSBMLP_MAX_LINE_LENGTH - 1);
    }
}

bool CustomGuiImageButton(Rectangle bounds, Texture2D texture) {
    DrawTexture(texture, bounds.x, bounds.y, WHITE);
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), bounds);
}

void query_playback_controls(UserInterface* ui, bool* stop, bool* play, bool* play_frame, bool* pause) {
    *stop = ui->stop_requested;
    *play = ui->play_requested;
    *play_frame = ui->play_frame_requested;
    *pause = ui->pause_requested;

    ui->stop_requested = false;
    ui->play_requested = false;
    ui->play_frame_requested = false;
    ui->pause_requested = false;
}

bool query_virtual_dims_change(UserInterface* ui, int* width, int* height) {
    if (!ui->change_virtual_dims_requested) return false;

    ui->change_virtual_dims_requested = false;

    *width = ui->new_virtual_dims[0];
    *height = ui->new_virtual_dims[1];

    return true;
}

bool query_xml_index_changed(UserInterface* ui, int* index) {
    if(!ui->active_xml_file_change_requested) return false;

    ui->active_xml_file_change_requested = false;

    *index = (ui->current_page * MKSBMLP_FILES_PER_PAGE) + ui->active_xml_file;

    return true;
}

void validate_width(UserInterface* ui) {
    ui->width_buffer_edit = false;

    int width = atoi(ui->width_buffer);
    if(width < 64) width = 64;
    else if(width > 4096) width = 4096;

    snprintf(ui->width_buffer, MKSBMLP_NUMBER_TEXT_WIDTH, "%d", width);
}

void validate_height(UserInterface* ui) {
    ui->height_buffer_edit = false;

    int height = atoi(ui->height_buffer);
    if(height < 64) height = 64;
    else if(height > 4096) height = 4096;

    snprintf(ui->height_buffer, MKSBMLP_NUMBER_TEXT_WIDTH, "%d", height);
}

void update_dims(UserInterface* ui) {
    ui->change_virtual_dims_requested = true;

    int width = atoi(ui->width_buffer);
    int height = atoi(ui->height_buffer);

    ui->new_virtual_dims[0] = width;
    ui->new_virtual_dims[1] = height;
}

void set_play_state(UserInterface* ui, char* play_state_text) {
    strncpy(ui->play_state_buffer, play_state_text, sizeof(char) * MKSBMLP_TEXT_WIDTH - 1);
}
