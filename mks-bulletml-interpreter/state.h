#ifndef STATE_H
#define STATE_H

#include "mksbmli-defines.h"
#include "playback.h"

typedef struct {
    MKSBMLI_PLAYBACK_HANDLE next_free_playback_handle;
    Playback playbacks[MKSBMLI_MAX_PLAYBACK_HANDLES];

    bool is_playing;
} State;

void init_state(State* state);
void shutdown_state(State* state);

void update_state(State* state);

int add_playback(State* state, const char* xml_filename, MKSBMLI_PLAYBACK_HANDLE* handle);
int remove_playback(State* state, MKSBMLI_PLAYBACK_HANDLE handle);
int count_playbacks(State* state);
int find_playback(State* state, MKSBMLI_PLAYBACK_HANDLE handle, Playback* playback);

void set_state_playing(State* state, MKSBMLI_PLAYBACK_HANDLE handle, bool flag);

#endif // STATE_H
