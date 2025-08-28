#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <stdbool.h>

#include "raylib.h"

#include "defines.h"
#include "mks-bulletml-interpreter.h"

typedef struct {
    char xml_filenames[MKSBMLI_MAX_PLAYBACK_HANDLES][MKSBMLI_XML_FILENAME_MAX_LENGTH];

    char loaded_xml_file_list[MKSBMLP_MAX_LINE_LENGTH];
    int nos_loaded_xmls_files;
    int active_xml_file;
    bool active_xml_file_edit_mode;

    Texture2D stop_button_texture;
    Texture2D play_button_texture;
    Texture2D play_frame_button_texture;
    Texture2D pause_button_texture;

    bool stop_requested;
    bool play_requested;
    bool play_frame_requested;
    bool pause_requested;
} UserInterface;

void init_user_interface(UserInterface* ui);
void render_user_interface(UserInterface* ui);

void extract_xml_shortnames(UserInterface* ui);
int count_loaded_xml_files(UserInterface* ui);

bool CustomGuiImageButton(Rectangle bounds, Texture2D texture);

void query_playback_controls(UserInterface* ui, bool* stop, bool* play, bool* play_frame, bool* pause);

#endif // USER_INTERFACE_H
