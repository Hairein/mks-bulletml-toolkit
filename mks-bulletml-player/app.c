#include <limits.h>

#include "mks-bulletml-interpreter.h"
#include "utility.h"
#include "app.h"

Color bg_color = {22, 22, 22, 255};
Color playfield_bg_color = {8, 8, 8, 255};

int init_app(App* app) {
    app->virtual_playfield_dims = (Vector2){240.0f, 320.0f};
    app->projected_playfield = (Rectangle){0.0f, 0.0f, 0.0f, 0.0f};

    for(int index = 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        app->playback_handles[index] = 0;
    }

    printf("\n");
    const char test1_xml_filename[] = "/home/mkoleoso/GitHub/mks-bulletml-toolkit/bulletml_files/basic_bml.xml";
    int add_result1 = add_xml_file(app, test1_xml_filename);
    printf("add xml file %s result: %d\n", test1_xml_filename, (int)add_result1);
    const char test2_xml_filename[] = "/home/mkoleoso/GitHub/mks-bulletml-toolkit/bulletml_files/inter_bml.xml";
    int add_result2 = add_xml_file(app, test2_xml_filename);
    printf("add xml file %s result: %d\n", test2_xml_filename, (int)add_result2);
    const char test3_xml_filename[] = "/home/mkoleoso/GitHub/mks-bulletml-toolkit/bulletml_files/aimed_bml.xml";
    int add_result3 = add_xml_file(app, test3_xml_filename);
    printf("add xml file %s result: %d\n", test3_xml_filename, (int)add_result3);
    printf("\n");

    app->is_playing = false;
    app->pause_after_frame = false;
    app->stop_playing = false;

    app->frame_counter = 0;

    mksbmli_set_random_seed(1337);

    app->current_active_playback_index = 0;
    mksbmli_start_playback(app->playback_handles[app->current_active_playback_index]);
    printf("start playing");

    return MKSBMLP_NO_ERROR;
}

void update_app(App* app) {
    handle_input(app);

    if(app->is_playing) {
        mksbmli_next_frame(app->playback_handles[app->current_active_playback_index]);

        printf("playing frame %d...\n", app->frame_counter++);
        if(app->frame_counter >= UINT_MAX) app->frame_counter -= UINT_MAX;
    }
}

void post_update_app(App* app) {
    if(app->pause_after_frame) {
        app->pause_after_frame = false;

        app->is_playing = false;

        printf("paused playing\n");
    }

    if(app->stop_playing) {
        app->stop_playing = false;

        mksbmli_stop_playback(app->playback_handles[app->current_active_playback_index]);
        app->frame_counter = 0;

        app->is_playing = false;

        printf("stopped playing\n");
    }
}

void render_app(App* app) {
    calculate_playfield(app);

    ClearBackground(bg_color);

    DrawRectangle(app->projected_playfield.x, app->projected_playfield.y, app->projected_playfield.width, app->projected_playfield.height, playfield_bg_color);
}

void handle_input(App* app)
{
    if(IsKeyReleased(KEY_Q)) {
        printf("Key Q - Stop Playing\n");

        app->stop_playing = true;
    }
    else if(IsKeyReleased(KEY_W)) {
        if(!app->is_playing) {
            printf("Key W - Play\n");

            app->is_playing = true;
        }
    }
    else if(IsKeyDown(KEY_E)) {
        if(!app->pause_after_frame) {
            printf("Key E - Play Frame\n");

            app->is_playing = true;
            app->pause_after_frame = true;
        }
    }
    else if(IsKeyReleased(KEY_R)) {
        if(app->is_playing && !app->pause_after_frame) {
            printf("Key R - Pause Playing\n");

            app->pause_after_frame = true;
        }
    }
}
