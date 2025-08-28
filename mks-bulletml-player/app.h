#ifndef APP_H
#define APP_H

#include "raylib.h"

#include "mks-bulletml-interpreter.h"

#define MKSBMLP_NO_ERROR 0
#define MKSBMLP_INVALID_PARAMETER -1
#define MKSBMLP_UNSPECIFIED_ERROR -2
#define MKSBMLP_TOO_MANY_XML_FILES -3

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
} App;

int init_app(App* app);
void update_app(App* app);
void post_update_app(App* app);
void render_app(App* app);

void handle_input(App* app);

#endif // APP_H
