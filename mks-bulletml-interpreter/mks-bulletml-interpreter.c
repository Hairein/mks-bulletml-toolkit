#include <libxml/parser.h>
#include <libxml/tree.h>

#include "platform-random.h"

#include "mks-bulletml-interpreter.h"
#include "state.h"

State state;

int mksbmli_init()
{
    char title[MKSBMLI_MAX_TEXT_LENGTH];
    snprintf(title, sizeof(title), "MKS BulletML Interpreter Library v%d.%d.%d startup.\n", MKSBMLI_MAJOR_VERSION, MKSBMLI_MINOR_VERSION, MKSBMLI_PATCH_VERSION);
    printf("%s", title);

    xmlInitParser();

    mksbmli_set_random_seed(1337);

    init_state(&state);

    return MKSBMLI_NO_ERROR;
}

int mksbmli_shutdown()
{
    shutdown_state(&state);

    xmlCleanupParser();

    printf("MKS BulletML Interpreter Library shutdown.\n");

    return MKSBMLI_NO_ERROR;
}

void mksbmli_get_version(int* major, int* minor, int* patch) {
    *major = MKSBMLI_MAJOR_VERSION;
    *minor = MKSBMLI_MINOR_VERSION;
    *patch = MKSBMLI_PATCH_VERSION;
}

int mksbmli_set_random_seed(int seed)
{
    INIT_RANDOM(seed);

    return MKSBMLI_NO_ERROR;
}

int mksbmli_load_xml(const char* xml_filename, MKSBMLI_PLAYBACK_HANDLE* handle) {
    return add_playback(&state, xml_filename, handle);
}

int mksbmli_unload_xml(MKSBMLI_PLAYBACK_HANDLE handle) {
    return remove_playback(&state, handle);
}

int mksbmli_count_xmls() {
    return count_playbacks(&state);
}

void mksbmli_start_playback(MKSBMLI_PLAYBACK_HANDLE handle) {
    set_state_playing(&state, handle, true);
}

void mksbmli_stop_playback(MKSBMLI_PLAYBACK_HANDLE handle) {
    set_state_playing(&state, handle, false);
}

void mksbmli_next_frame(MKSBMLI_PLAYBACK_HANDLE handle) {
    update_state(&state);
}

int mksbmli_get_bullets(MKSBMLI_PLAYBACK_HANDLE handle, int max_bullets, VirtualBullet** bullets, int* nos_bullets) {
    for(int index = 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        if(state.playbacks[index].handle == handle) {
            get_bullets(&state.playbacks[index], max_bullets, bullets, nos_bullets);

            return MKSBMLI_NO_ERROR;
        }
    }

    return MKSBMLI_INVALID_HANDLE;
}

int mksbmli_delete_bullets(MKSBMLI_PLAYBACK_HANDLE handle, MKSBMLI_BULLET_HANDLE* bullet_handles, int nos_bullet_handles) {
    for(int index = 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        if(state.playbacks[index].handle == handle) {
            destroy_bullets(&state.playbacks[index], bullet_handles, nos_bullet_handles);

            return MKSBMLI_NO_ERROR;
        }
    }

    return MKSBMLI_INVALID_HANDLE;
}

int mksbmli_clear_bullets(MKSBMLI_PLAYBACK_HANDLE handle) {
    for(int index = 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        if(state.playbacks[index].handle == handle) {
            clear_bullets(&state.playbacks[index]);

            return MKSBMLI_NO_ERROR;
        }
    }

    return MKSBMLI_INVALID_HANDLE;
}

int mksbmli_set_player_position(MKSBMLI_PLAYBACK_HANDLE handle, Vector2 player_position) {
    for(int index = 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        if(state.playbacks[index].handle == handle) {
            set_player_position(&state.playbacks[index], player_position);

            return MKSBMLI_NO_ERROR;
        }
    }

    return MKSBMLI_INVALID_HANDLE;
}

int mksbmli_set_rank(MKSBMLI_PLAYBACK_HANDLE handle, float rank) {
    for(int index = 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        if(state.playbacks[index].handle == handle) {
            set_rank(&state.playbacks[index], rank);

            return MKSBMLI_NO_ERROR;
        }
    }

    return MKSBMLI_INVALID_HANDLE;
}

