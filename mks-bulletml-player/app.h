#ifndef APP_H
#define APP_H

#include "mks-bulletml-interpreter.h"
#include "user_interface.h"

typedef struct {
    Vector2 virtual_playfield_dims;
    Rectangle projected_playfield;

    const char xml_filenames[MKSBMLI_MAX_PLAYBACK_HANDLES][MKSBMLI_XML_FILENAME_MAX_LENGTH];
    MKSBMLI_PLAYBACK_HANDLE playback_handles[MKSBMLI_MAX_PLAYBACK_HANDLES];
    unsigned int current_active_playback_index;

    bool is_playing;
    bool pause_after_frame;
    bool stop_playing;

    unsigned int frame_counter;

    UserInterface ui;
} App;

int init_app(App* app);
void update_app(App* app);
void post_update_app(App* app);
void render_app(App* app);

void handle_app_input(App* app);
void handle_ui_input(App* app);

#endif // APP_H
