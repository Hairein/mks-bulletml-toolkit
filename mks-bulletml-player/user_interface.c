#include <string.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "user_interface.h"

void init_user_interface(UserInterface* ui) {
    memset(ui, 0, sizeof(UserInterface));

    strncpy(ui->xml_filenames[0], "/home/mkoleoso/GitHub/mks-bulletml-toolkit/bulletml_files/basic_bml.xml", MKSBMLI_XML_FILENAME_MAX_LENGTH - 1);
    strncpy(ui->xml_filenames[1], "/home/mkoleoso/GitHub/mks-bulletml-toolkit/bulletml_files/inter_bml.xml", MKSBMLI_XML_FILENAME_MAX_LENGTH - 1);
    strncpy(ui->xml_filenames[2], "/home/mkoleoso/GitHub/mks-bulletml-toolkit/bulletml_files/aimed_bml.xml", MKSBMLI_XML_FILENAME_MAX_LENGTH - 1);

    ui->nos_loaded_xmls_files = count_loaded_xml_files(ui);

    ui->active_xml_file = 0;
    ui->active_xml_file_edit_mode = false;

    extract_xml_shortnames(ui);

    ui->stop_button_texture = LoadTexture("../assets/button_stop.png");
    ui->play_button_texture = LoadTexture("../assets/button_play.png");
    ui->play_frame_button_texture = LoadTexture("../assets/button_play_frame.png");
    ui->pause_button_texture = LoadTexture("../assets/button_pause.png");

    ui->stop_requested = false;
    ui->play_requested = false;
    ui->play_frame_requested = false;
    ui->pause_requested = false;
}

void render_user_interface(UserInterface* ui) {
    if (ui->nos_loaded_xmls_files < MKSBMLI_MAX_PLAYBACK_HANDLES) {
        GuiButton((Rectangle){ 0, 0, 96, 30 }, "Add XML File...");
    }

    if (ui->nos_loaded_xmls_files > 0) {
        if (GuiDropdownBox((Rectangle){ 96, 0, 256, 30 }, ui->loaded_xml_file_list, &ui->active_xml_file, ui->active_xml_file_edit_mode))
        {
            ui->active_xml_file_edit_mode = !ui->active_xml_file_edit_mode;
        }
    }

    int panel_start_y = GetScreenHeight() - (int)TOP_BOTTOM_CUTOFF_HALF;
    if(CustomGuiImageButton((Rectangle){ 30, panel_start_y, 30, 30 }, ui->stop_button_texture)) ui->stop_requested |= true;
    if(CustomGuiImageButton((Rectangle){ 60, panel_start_y, 30, 30 }, ui->play_button_texture)) ui->play_requested |= true;
    if(CustomGuiImageButton((Rectangle){ 90, panel_start_y, 30, 30 }, ui->play_frame_button_texture)) ui->play_frame_requested |= true;
    if(CustomGuiImageButton((Rectangle){ 120, panel_start_y, 30, 30 }, ui->pause_button_texture)) ui->pause_requested |= true;
}

void extract_xml_shortnames(UserInterface* ui) {
    memset(ui->loaded_xml_file_list, 0, MKSBMLP_MAX_LINE_LENGTH);

    for (int index = 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        const char* filename = ui->xml_filenames[index];

        if (filename[0] == '\0') continue;

        const char* shortname = strrchr(filename, '/');
        if (!shortname) shortname = strrchr(filename, '\\');  // Windows-Pfad
        if (shortname) shortname++; else shortname = filename;

        if (ui->loaded_xml_file_list[0] != '\0') {
            strncat(ui->loaded_xml_file_list, ";", MKSBMLP_MAX_LINE_LENGTH - strlen(ui->loaded_xml_file_list) - 1);
        }

        strncat(ui->loaded_xml_file_list, shortname, MKSBMLP_MAX_LINE_LENGTH - strlen(ui->loaded_xml_file_list) - 1);
    }
}

int count_loaded_xml_files(UserInterface* ui) {
    int total = 0;

    for(int index= 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        if(strncmp(ui->xml_filenames[index],"", MKSBMLI_XML_FILENAME_MAX_LENGTH) == 0) continue;

        total++;
    }

    return total;
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
