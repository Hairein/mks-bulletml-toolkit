#include <limits.h>
#include <string.h>
#include <stdio.h>

#include "mks-bulletml-interpreter.h"
#include "utility.h"
#include "app.h"

Color bg_color = {22, 22, 22, 255};
Color playfield_bg_color = {8, 8, 8, 255};

int init_app(App* app) {
    memset(app, 0, sizeof(App));

    app->xml_count = 0;
    load_xml_filenames("../bulletml_files", app->xml_filenames, &app->xml_count);
    if(app->xml_count > 0) {
        // printf("\n");
        for (int i = 0; i < app->xml_count; i++) {
            int add_result = add_xml_file(app, app->xml_filenames[i]);
            // printf("add xml file %s result: %d\n", app->xml_filenames[i], (int)add_result);
        }
        // printf("\n");
    }

    app->bullet_texture = LoadTexture("../assets/textures/8x8_default_bullet.png");
    app->player_cursor_texture = LoadTexture("../assets/textures/player_cursor.png");

    app->virtual_playfield_dims = (Vector2){320.0f, 240.0f};
    app->projected_playfield = (Rectangle){0.0f, 0.0f, 0.0f, 0.0f};
    app->player_position = (Vector2){0.0f, 0.0f};
    app->emitter_center = (Vector2){0.0f, 0.0f};

    app->is_playing = false;
    app->pause_after_frame = false;
    app->start_playing = false;
    app->stop_playing = false;
    app->rewind_playback = true;

    app->frame_counter = 0;

    mksbmli_set_random_seed(1337);

    if(app->xml_count > 0) {
        init_user_interface(&app->ui, app->xml_filenames, app->xml_count,app->virtual_playfield_dims.x, app->virtual_playfield_dims.y);

        app->current_active_playback_index = 0;
    }

    mksbmli_set_emitter_center(app->playback_handles[app->current_active_playback_index],
        app->emitter_center.x, app->emitter_center.y);

    strncpy(app->stopped_text, "Stopped", MKSBMLP_TEXT_WIDTH - 1);
    strncpy(app->playing_text, "Playing", MKSBMLP_TEXT_WIDTH - 1);
    strncpy(app->paused_text, "Paused", MKSBMLP_TEXT_WIDTH - 1);

    set_play_state(&app->ui, app->stopped_text);

    return MKSBMLP_NO_ERROR;
}

void update_app(App* app) {
    if(app->xml_count == 0) return;

    handle_app_input(app);
    handle_ui_input(app);

    if(app->start_playing) {
        app->start_playing = false;

        if(app->rewind_playback) {
            app->rewind_playback = false;

            mksbmli_start_playback(app->playback_handles[app->current_active_playback_index]);
            //printf("start playing\n");

        }

        app->is_playing = true;

        set_play_state(&app->ui, app->playing_text);
    }

    Vector2 screen_half = (Vector2){GetScreenWidth() / 2.0, GetScreenHeight() / 2.0f};
    Vector2 mouse_position = GetMousePosition();
    Vector2 virtual_half = (Vector2){app->virtual_playfield_dims.x / 2.0f, app->virtual_playfield_dims.y / 2.0f};
    float projected_scale = app->projected_playfield.width / app->virtual_playfield_dims.x;
    Vector2 min_edges = (Vector2){
        screen_half.x - (virtual_half.x * projected_scale),
        screen_half.y - (virtual_half.y * projected_scale)
    };
    Vector2 max_edges = (Vector2){
        screen_half.x + (virtual_half.x * projected_scale),
        screen_half.y + (virtual_half.y * projected_scale)
    };
    app->player_position = (Vector2){0.0f, -virtual_half.y + (virtual_half.y * 0.33f)};
    if(mouse_position.x >= min_edges.x && mouse_position.x <= max_edges.x
        && mouse_position.y >= min_edges.y && mouse_position.y <= max_edges.y) {
        app->player_position = (Vector2){
            (mouse_position.x - screen_half.x) / projected_scale,
            (-mouse_position.y + screen_half.y) / projected_scale,
        };
    }
    mksbmli_set_player_position(app->playback_handles[app->current_active_playback_index], app->player_position.x, app->player_position.y);
    //printf("Player: x(%d), y(%d)\n", (int)app->player_position.x, (int)app->player_position.y);

    if(app->is_playing) {
        mksbmli_next_frame(app->playback_handles[app->current_active_playback_index]);

        //printf("playing frame %d...\n", app->frame_counter++);
        if(app->frame_counter >= UINT_MAX) app->frame_counter -= UINT_MAX;
    }
}

void post_update_app(App* app) {
    if(app->xml_count == 0) return;

    if(app->pause_after_frame) {
        app->pause_after_frame = false;

        app->is_playing = false;

        //printf("paused playing\n");

        set_play_state(&app->ui, app->paused_text);
    }

    if(app->stop_playing) {
        app->stop_playing = false;

        mksbmli_stop_playback(app->playback_handles[app->current_active_playback_index]);
        mksbmli_clear_bullets(app->playback_handles[app->current_active_playback_index]);
        app->frame_counter = 0;

        app->is_playing = false;
        app->rewind_playback = true;

        //printf("stopped playing\n");

        set_play_state(&app->ui, app->stopped_text);
    }

    int xml_index;
    if(query_xml_index_changed(&app->ui, &xml_index)) {
        mksbmli_stop_playback(app->playback_handles[app->current_active_playback_index]);
        mksbmli_clear_bullets(app->playback_handles[app->current_active_playback_index]);
        app->frame_counter = 0;

        app->is_playing = false;
        app->rewind_playback = true;

        //printf("stopped playing xml file index: %d\n", app->current_active_playback_index);

        app->current_active_playback_index = xml_index;
        //printf("selected xml file index: %d\n", app->current_active_playback_index);

        reset_emitter_center(app);

        set_play_state(&app->ui, app->stopped_text);
    }

    int new_width, new_height;
    if(query_virtual_dims_change(&app->ui, &new_width, &new_height)) {
        app->virtual_playfield_dims = (Vector2){new_width, new_height};

        reset_emitter_center(app);
    }
}

void render_app(App* app) {
    if(app->xml_count == 0) return;

    calculate_playfield(app);

    ClearBackground(bg_color);

    DrawRectangle(app->projected_playfield.x, app->projected_playfield.y, app->projected_playfield.width, app->projected_playfield.height, playfield_bg_color);

    Vector2 screen_half = (Vector2){GetScreenWidth() / 2.0, GetScreenHeight() / 2.0f};
    float projected_scale = app->projected_playfield.width / app->virtual_playfield_dims.x;

    int emitter_position_x = (int)(screen_half.x + (app->emitter_center.x * projected_scale) - (app->player_cursor_texture.width / 2.0f));
    int emitter_position_y = (int)(screen_half.y + (app->emitter_center.y * projected_scale) - (app->player_cursor_texture.height / 2.0f));
    DrawTexture(app->player_cursor_texture, emitter_position_x, emitter_position_y, DARKGRAY);

    VirtualBullet* bullets[MKSBMLI_MAX_BULLETS];
    int nos_bullets = 0;
    MKSBMLI_BULLET_HANDLE delete_bullets[MKSBMLI_MAX_BULLETS];
    int nos_delete_bullets = 0;
    mksbmli_get_bullets(app->playback_handles[app->current_active_playback_index], MKSBMLI_MAX_BULLETS, bullets, &nos_bullets);
    if(nos_bullets > 0) {
        Vector2 texture_half = (Vector2){app->bullet_texture.width / 2.0f, app->bullet_texture.height / 2.0f};
        Vector2 bullet_offset = (Vector2){screen_half.x - texture_half.x, screen_half.y - texture_half.y};
        Vector2 virtual_half = (Vector2){app->virtual_playfield_dims.x / 2.0f, app->virtual_playfield_dims.y / 2.0f};

        Vector2 min_edges = (Vector2){
            screen_half.x - (virtual_half.x * projected_scale),
            screen_half.y - (virtual_half.y * projected_scale)
        };
        Vector2 max_edges = (Vector2){
            screen_half.x + (virtual_half.x * projected_scale),
            screen_half.y + (virtual_half.y * projected_scale)
        };

        for(int index = 0; index < nos_bullets; index++) {
            VirtualBullet* bullet = bullets[index];

            Vector2 position = (Vector2){
                bullet_offset.x + (bullet->position.x * projected_scale),
                bullet_offset.y + (bullet->position.y * projected_scale)
            };

            DrawTexture(app->bullet_texture, (int)position.x, (int)position.y, WHITE);

            if(position.x <  min_edges.x || position.x > max_edges.x
                || position.y < min_edges.y || position.y > max_edges.y) {
                delete_bullets[nos_delete_bullets++] = bullet->handle;
            }
        }
    }

    int player_cursor_position_x = (int)(screen_half.x + (app->player_position.x * projected_scale) - (app->player_cursor_texture.width / 2.0f));
    int player_cursor_position_y = (int)(screen_half.y - (app->player_position.y * projected_scale) - (app->player_cursor_texture.height / 2.0f));
    DrawTexture(app->player_cursor_texture, player_cursor_position_x, player_cursor_position_y, YELLOW);

    char nos_bullets_text[128];
    snprintf(nos_bullets_text, 128, "Player XY(%6d, %6d), Nos Bullets: %4d",
             (int)app->player_position.x, (int)app->player_position.y,
            nos_bullets);
    DrawText(nos_bullets_text, 8, 34, 12, DARKGRAY);

    if(nos_delete_bullets > 0) {
        mksbmli_delete_bullets(app->playback_handles[app->current_active_playback_index], delete_bullets, nos_delete_bullets);
    }

    render_user_interface(&app->ui);
}

void handle_app_input(App* app)
{
    if(IsKeyReleased(KEY_Q)) {
        app->stop_playing = true;
    }
    else if(IsKeyReleased(KEY_W)) {
        if(!app->is_playing) {
            app->start_playing = true;
        }
    }
    else if(IsKeyDown(KEY_E)) {
        if(!app->pause_after_frame) {
            app->start_playing = true;
            app->pause_after_frame = true;
        }
    }
    else if(IsKeyReleased(KEY_R)) {
        if(app->is_playing && !app->pause_after_frame) {
            app->pause_after_frame = true;
        }
    }

    if(IsKeyDown(KEY_C) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        set_emitter_center(app);
    } else if(IsKeyPressed(KEY_X)) {
        reset_emitter_center(app);
    }
}

void handle_ui_input(App* app) {
    bool stop_requested;
    bool play_requested;
    bool play_frame_requested;
    bool pause_requested;
    query_playback_controls(&app->ui, &stop_requested, &play_requested, &play_frame_requested, &pause_requested);

    if(stop_requested) app->stop_playing = true;
    if(play_requested && !app->is_playing) app->start_playing = true;
    if(play_frame_requested && !app->pause_after_frame) {
        app->start_playing = true;
        app->pause_after_frame = true;
    }
    if(pause_requested && app->is_playing && !app->pause_after_frame) app->pause_after_frame = true;
}

void set_emitter_center(App* app) {
    Vector2 mouse_position = GetMousePosition();

    Vector2 screen_half = (Vector2){GetScreenWidth() / 2.0, GetScreenHeight() / 2.0f};
    float projected_scale = app->projected_playfield.width / app->virtual_playfield_dims.x;

    Vector2 relative_center = (Vector2){
        (mouse_position.x - screen_half.x) / projected_scale,
        (mouse_position.y - screen_half.y) / projected_scale,
    };

    Vector2 virtual_half = (Vector2){app->virtual_playfield_dims.x / 2.0f, app->virtual_playfield_dims.y / 2.0f};

    if (relative_center.x >= -virtual_half.x && relative_center.x <= virtual_half.x
        && relative_center.y >= -virtual_half.y && relative_center.y <= virtual_half.y) {
        app->emitter_center.x = relative_center.x;
        app->emitter_center.y = relative_center.y;

        mksbmli_set_emitter_center(app->playback_handles[app->current_active_playback_index],
                                   app->emitter_center.x, app->emitter_center.y);
    }
}

void reset_emitter_center(App* app) {
    app->emitter_center = (Vector2){ 0.0f, 0.0f };

    mksbmli_set_emitter_center(app->playback_handles[app->current_active_playback_index],
        app->emitter_center.x, app->emitter_center.y);
}
