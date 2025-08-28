#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "user_interface.h"

void init_user_interface(UserInterface* ui) {
    memset(ui, 0, sizeof(UserInterface));

    char xml_files[MKSBMLI_MAX_PLAYBACK_HANDLES][MKSBMLI_XML_FILENAME_MAX_LENGTH];
    int xml_count = 0;
    load_xml_filenames("../bulletml_files", xml_files, &xml_count);

    if(xml_count > 0) {
        for (int i = 0; i < xml_count; i++) {
            printf("XML-file %d: %s\n", i + 1, xml_files[i]);
            strncpy(ui->xml_filenames[i], xml_files[i], MKSBMLI_XML_FILENAME_MAX_LENGTH - 1);
        }

        ui->nos_loaded_xmls_files = count_loaded_xml_files(ui);

        ui->active_xml_file = 0;
        ui->active_xml_file_edit_mode = false;

        extract_xml_shortnames(ui);
    }

    ui->stop_button_texture = LoadTexture("../assets/textures/button_stop.png");
    ui->play_button_texture = LoadTexture("../assets/textures/button_play.png");
    ui->play_frame_button_texture = LoadTexture("../assets/textures/button_play_frame.png");
    ui->pause_button_texture = LoadTexture("../assets/textures/button_pause.png");

    ui->stop_requested = false;
    ui->play_requested = false;
    ui->play_frame_requested = false;
    ui->pause_requested = false;

    ui->change_virtual_dims_requested = false;
    int new_virtual_dims[2] = {0, 0};
}

void render_user_interface(UserInterface* ui) {
    if (ui->nos_loaded_xmls_files > 0) {
        if (GuiDropdownBox((Rectangle){ 0, 0, 256, 30 }, ui->loaded_xml_file_list, &ui->active_xml_file, ui->active_xml_file_edit_mode))
        {
            ui->active_xml_file_edit_mode = !ui->active_xml_file_edit_mode;
        }

        int panel_start_y = GetScreenHeight() - (int)TOP_BOTTOM_CUTOFF_HALF;
        if(CustomGuiImageButton((Rectangle){ 0, panel_start_y, 30, 30 }, ui->stop_button_texture)) ui->stop_requested |= true;
        if(CustomGuiImageButton((Rectangle){ 30, panel_start_y, 30, 30 }, ui->play_button_texture)) ui->play_requested |= true;
        if(CustomGuiImageButton((Rectangle){ 60, panel_start_y, 30, 30 }, ui->play_frame_button_texture)) ui->play_frame_requested |= true;
        if(CustomGuiImageButton((Rectangle){ 90, panel_start_y, 30, 30 }, ui->pause_button_texture)) ui->pause_requested |= true;
    }
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

bool query_virtual_dims_change(UserInterface* ui, int* width, int* height) {
    if (!ui->change_virtual_dims_requested) return false;

    ui->change_virtual_dims_requested = false;

    if(ui->new_virtual_dims[0] < 64) ui->new_virtual_dims[0] = 64;
    if(ui->new_virtual_dims[0] > 4096) ui->new_virtual_dims[0] = 4096;
    if(ui->new_virtual_dims[1] < 64) ui->new_virtual_dims[1] = 64;
    if(ui->new_virtual_dims[1] > 4096) ui->new_virtual_dims[1] = 4096;

    *width = ui->new_virtual_dims[0];
    *height = ui->new_virtual_dims[1];

    return true;
}

void load_xml_filenames(const char* folder_path, char filenames[MKSBMLI_MAX_PLAYBACK_HANDLES][MKSBMLI_XML_FILENAME_MAX_LENGTH], int* count) {
    DIR* dir = opendir(folder_path);
    if (!dir) {
        perror("Can't open folder");
        return;
    }

    struct dirent* entry;
    *count = 0;

    while ((entry = readdir(dir)) != NULL && *count < MKSBMLI_MAX_PLAYBACK_HANDLES) {
        if (entry->d_type == DT_REG) {
            const char* name = entry->d_name;
            const char* ext = strrchr(name, '.');
            if (ext && strcmp(ext, ".xml") == 0) {
                snprintf(filenames[*count], MKSBMLI_XML_FILENAME_MAX_LENGTH, "%s/%s", folder_path, name);
                (*count)++;
            }
        }
    }

    closedir(dir);
}
