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
} App;

int init_app(App* app);
int update_app(App* app);
int render_app(App* app);

int set_playfield_dims(App* app, int width, int height);

int add_xml_file(App* app, const char* xml_filename);

void calculate_playfield(App* app);

#endif // APP_H
