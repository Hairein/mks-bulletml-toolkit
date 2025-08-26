#include "state.h"

void init_state(State* state) {
    state->next_free_playback_handle = 1;

    for(int index = 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        init_playback(&state->playbacks[index], "", (MKSBMLI_PLAYBACK_HANDLE)0);
    }
}

void shutdown_state(State* state) {
    for(int index = 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        shutdown_playback(&state->playbacks[index]);
    }
}

int update_state(State* state) {
    for(int index = 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        if(state->playbacks[index].handle == 0) continue;

        update_playback(&state->playbacks[index]);
    }

    return MKSBMLI_NO_ERROR;
}

int add_playback(State* state, const char* xml_filename, MKSBMLI_PLAYBACK_HANDLE* handle) {
    for(int index = 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        if(state->playbacks[index].handle == 0) {
            MKSBMLI_PLAYBACK_HANDLE new_handle = state->next_free_playback_handle++;

            if(init_playback(&state->playbacks[index], xml_filename, new_handle) == MKSBMLI_NO_ERROR) {
                *handle = (MKSBMLI_PLAYBACK_HANDLE)new_handle;

                return MKSBMLI_NO_ERROR;
            }

            return MKSBMLI_UNSPECIFIED_ERROR;
        }
    }

    return MKSBMLI_TOO_MANY_XML_FILES;
}

int remove_playback(State* state, MKSBMLI_PLAYBACK_HANDLE handle) {
    for(int index = 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        if(state->playbacks[index].handle != handle) continue;

        init_playback(&state->playbacks[index], "", (MKSBMLI_PLAYBACK_HANDLE)0);

        return MKSBMLI_NO_ERROR;
    }

    return MKSBMLI_INVALID_HANDLE;
}

int count_playbacks(State* state) {
    int total = 0;

    for(int index = 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        if(state->playbacks[index].handle == 0) continue;

        total++;
    }

    return total;
}

int find_playback(State* state, MKSBMLI_PLAYBACK_HANDLE handle, Playback* playback) {
    for(int index = 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        if(state->playbacks[index].handle != handle) continue;

        playback = &state->playbacks[index];

        return MKSBMLI_NO_ERROR;
    }

    return MKSBMLI_INVALID_HANDLE;
}
