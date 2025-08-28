#include <libxml/parser.h>
#include <libxml/tree.h>

#include "platform-random.h"

#include "mks-bulletml-interpreter.h"
#include "state.h"

State state;

int mksbmli_init()
{
    printf("MKS BulletML Interpreter Library v0.0.1 startup.\n");

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

int mksbmli_start_playback(MKSBMLI_PLAYBACK_HANDLE handle) {
    return MKSBMLI_NO_ERROR;
}

int mksbmli_stop_playback(MKSBMLI_PLAYBACK_HANDLE handle) {
    return MKSBMLI_NO_ERROR;
}

int mksbmli_is_playing(MKSBMLI_PLAYBACK_HANDLE handle) {
    return MKSBMLI_NO_ERROR;
}

int mksbmli_next_frame(MKSBMLI_PLAYBACK_HANDLE handle) {
    return MKSBMLI_NO_ERROR;
}

int mksbmli_get_bullets() {
    return MKSBMLI_NO_ERROR;
}

int mksbmli_delete_bullets() {
    return MKSBMLI_NO_ERROR;
}

