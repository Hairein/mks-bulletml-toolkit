#ifndef MKSBULLETMLINTERPRETER_H
#define MKSBULLETMLINTERPRETER_H

#include <stdbool.h>

#include "mksbmli-defines.h"
#include "virtual_bullet.h"

#ifdef _WIN32
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT __attribute__((visibility("default")))
#endif

#define MKSBMLI_MAJOR_VERSION 0
#define MKSBMLI_MINOR_VERSION 8
#define MKSBMLI_PATCH_VERSION 0

EXPORT int mksbmli_init();
EXPORT int mksbmli_shutdown();
EXPORT void mksbmli_get_version(int* major, int* minor, int* patch);

EXPORT int mksbmli_set_random_seed(int seed);

EXPORT int mksbmli_load_xml(const char* xml_filename, MKSBMLI_PLAYBACK_HANDLE* handle);
EXPORT int mksbmli_unload_xml(MKSBMLI_PLAYBACK_HANDLE handle);
EXPORT int mksbmli_count_xmls();

EXPORT void mksbmli_start_playback(MKSBMLI_PLAYBACK_HANDLE handle);
EXPORT void mksbmli_stop_playback(MKSBMLI_PLAYBACK_HANDLE handle);
EXPORT void mksbmli_next_frame(MKSBMLI_PLAYBACK_HANDLE handle);

EXPORT int mksbmli_get_bullets(MKSBMLI_PLAYBACK_HANDLE handle, int max_bullets, VirtualBullet** bullets, int* nos_bullets);
EXPORT int mksbmli_delete_bullets(MKSBMLI_PLAYBACK_HANDLE handle, MKSBMLI_BULLET_HANDLE* bullet_handles, int nos_bullet_handles);
EXPORT int mksbmli_clear_bullets(MKSBMLI_PLAYBACK_HANDLE handle);

EXPORT int mksbmli_set_player_position(MKSBMLI_PLAYBACK_HANDLE handle, Vector2 player_position);

#endif // MKSBULLETMLINTERPRETER_H
