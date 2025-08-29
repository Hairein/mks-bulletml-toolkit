#include <libxml/parser.h>
#include <libxml/tree.h>

#include "platform-random.h"

#include "mks-bulletml-interpreter.h"
#include "state.h"

State state;

int mksbmli_init()
{
    char title[MKSBMLI_MAX_TEXT_LENGTH];
    sprintf(title, "MKS BulletML Interpreter Library v%d.%d.%d startup.\n", MKSBMLI_MAJOR_VERSION, MKSBMLI_MINOR_VERSION, MKSBMLI_PATCH_VERSION);
    printf(title);

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

int mksbmli_get_version(int* major, int* minor, int* patch) {
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
    set_state_playing(&state, true);
}

void mksbmli_stop_playback(MKSBMLI_PLAYBACK_HANDLE handle) {
    set_state_playing(&state, false);
}

void mksbmli_next_frame(MKSBMLI_PLAYBACK_HANDLE handle) {
    update_state(&state);
}

int mksbmli_get_bullets() {
    return MKSBMLI_NO_ERROR;
}

int mksbmli_delete_bullets() {
    return MKSBMLI_NO_ERROR;
}

