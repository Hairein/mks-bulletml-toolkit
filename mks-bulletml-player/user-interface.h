#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <stdbool.h>

#include "raylib.h"

#include "mksbmlp-defines.h"
#include "mks-bulletml-interpreter.h"

#define MKSBMLP_FILES_PER_PAGE 15

typedef struct {
    char xml_filenames_copy[MKSBMLI_MAX_PLAYBACK_HANDLES][MKSBMLI_XML_FILENAME_MAX_LENGTH];

    char displayed_xml_file_list[MKSBMLP_MAX_LINE_LENGTH];
    int nos_loaded_xmls_files;
    int active_xml_file;
    int active_xml_file_change_requested;
    bool active_xml_file_edit_mode;

    Texture2D stop_button_texture;
    Texture2D play_button_texture;
    Texture2D play_frame_button_texture;
    Texture2D pause_button_texture;

    bool stop_requested;
    bool play_requested;
    bool play_frame_requested;
    bool pause_requested;

    bool change_virtual_dims_requested;
    int new_virtual_dims[2];

    char width_buffer[MKSBMLP_NUMBER_TEXT_WIDTH];
    bool width_buffer_edit;
    char height_buffer[MKSBMLP_NUMBER_TEXT_WIDTH];
    bool height_buffer_edit;

    int current_page;
    int nos_pages;

    char play_state_buffer[MKSBMLP_TEXT_WIDTH];
} UserInterface;

void init_user_interface(UserInterface* ui, char xml_filenames[MKSBMLI_MAX_PLAYBACK_HANDLES][MKSBMLI_XML_FILENAME_MAX_LENGTH], int xml_count,
    int v_width, int v_height);
void render_user_interface(UserInterface* ui);

void prepare_displayed_file_list(UserInterface* ui);
void build_displayed_file_list(UserInterface* ui);

int count_loaded_xml_files(UserInterface* ui);

bool CustomGuiImageButton(Rectangle bounds, Texture2D texture);

void query_playback_controls(UserInterface* ui, bool* stop, bool* play, bool* play_frame, bool* pause);
bool query_virtual_dims_change(UserInterface* ui, int* width, int* height);
bool query_xml_index_changed(UserInterface* ui, int* index);

void validate_width(UserInterface* ui);
void validate_height(UserInterface* ui);
void update_dims(UserInterface* ui);

void previous_page(UserInterface* ui);
void next_page(UserInterface* ui);

void copy_xml_filenames(UserInterface* ui, char xml_filenames[MKSBMLI_MAX_PLAYBACK_HANDLES][MKSBMLI_XML_FILENAME_MAX_LENGTH]);

void set_play_state(UserInterface* ui, char* play_state_text);

#endif // USER_INTERFACE_H
