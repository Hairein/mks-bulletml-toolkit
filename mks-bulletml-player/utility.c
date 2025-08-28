#include <string.h>

#include "defines.h"
#include "utility.h"

void set_playfield_dims(App* app, int width, int height)
{
    if(width < 64) width = 64;
    if(width > 4096) width = 4096;
    if(height < 64) height = 64;
    if(height > 4096) height = 4096;

    app->virtual_playfield_dims = (Vector2){width, height};
}

int add_xml_file(App* app, const char* xml_filename){
    MKSBMLI_PLAYBACK_HANDLE handle;
    int load_result = mksbmli_load_xml(xml_filename, &handle);
    if(load_result != MKSBMLP_NO_ERROR) return load_result;
    if(handle == 0) return MKSBMLP_UNSPECIFIED_ERROR;

    for(int index = 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        if(app->playback_handles[index] != 0) continue;

        memset((void*)&app->xml_filenames[index], 0, MKSBMLI_XML_FILENAME_MAX_LENGTH);
        strncpy((char*)&app->xml_filenames[index], xml_filename, MKSBMLI_XML_FILENAME_MAX_LENGTH);
        app->playback_handles[index] = handle;

        return MKSBMLP_NO_ERROR;
    }

    return MKSBMLP_TOO_MANY_XML_FILES;
}

void calculate_playfield(App* app) {
    float screen_width = GetScreenWidth();
    float screen_height = GetScreenHeight() - TOP_BOTTOM_CUTOFF;

    float virtual_width_ratio = app->virtual_playfield_dims.x / app->virtual_playfield_dims.y;

    float virtual_width = screen_width;
    float virtual_height = screen_width / virtual_width_ratio;

    if(virtual_height > screen_height) {
        float virtual_height_ratio = app->virtual_playfield_dims.y / screen_height;
        virtual_width = app->virtual_playfield_dims.x / virtual_height_ratio;
        virtual_height = screen_height;
    }

    app->projected_playfield.x = screen_width / 2.0f - (virtual_width / 2.0f);
    app->projected_playfield.y = screen_height / 2.0f - (virtual_height / 2.0f) + TOP_BOTTOM_CUTOFF_HALF;
    app->projected_playfield.width = virtual_width;
    app->projected_playfield.height = virtual_height;
}
