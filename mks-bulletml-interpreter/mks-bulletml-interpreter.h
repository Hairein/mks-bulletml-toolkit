#ifndef MKSBULLETMLINTERPRETER_H
#define MKSBULLETMLINTERPRETER_H

#include <stdbool.h>

#include "mksbmli-defines.h"
#include "virtual_bullet.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
  #define MKSBMLI_API __declspec(dllexport)
#else
  #define MKSBMLI_API __attribute__((visibility("default")))
#endif

#ifndef EXPORT
#define EXPORT MKSBMLI_API
#endif

#define MKSBMLI_MAJOR_VERSION 0
#define MKSBMLI_MINOR_VERSION 9
#define MKSBMLI_PATCH_VERSION 1

EXPORT int mksbmli_init(void);
EXPORT int mksbmli_shutdown(void);
EXPORT void mksbmli_get_version(int* major, int* minor, int* patch);

EXPORT int mksbmli_set_random_seed(int seed);

EXPORT int mksbmli_load_xml(const char* xml_filename, MKSBMLI_PLAYBACK_HANDLE* handle);
EXPORT int mksbmli_unload_xml(MKSBMLI_PLAYBACK_HANDLE handle);
EXPORT int mksbmli_count_xmls(void);

EXPORT void mksbmli_start_playback(MKSBMLI_PLAYBACK_HANDLE handle);
EXPORT void mksbmli_stop_playback(MKSBMLI_PLAYBACK_HANDLE handle);
EXPORT void mksbmli_next_frame(MKSBMLI_PLAYBACK_HANDLE handle);

EXPORT int mksbmli_get_bullets(MKSBMLI_PLAYBACK_HANDLE handle, int max_bullets, VirtualBullet** bullets, int* nos_bullets);
EXPORT int mksbmli_delete_bullets(MKSBMLI_PLAYBACK_HANDLE handle, MKSBMLI_BULLET_HANDLE* bullet_handles, int nos_bullet_handles);
EXPORT int mksbmli_clear_bullets(MKSBMLI_PLAYBACK_HANDLE handle);

EXPORT int mksbmli_set_emitter_center(MKSBMLI_PLAYBACK_HANDLE handle, float emitter_position_x, float emitter_position_y);
EXPORT int mksbmli_set_player_position(MKSBMLI_PLAYBACK_HANDLE handle, float player_position_x, float player_position_y);
EXPORT int mksbmli_set_rank(MKSBMLI_PLAYBACK_HANDLE handle, float rank);

#ifdef __cplusplus
}
#endif

#endif // MKSBULLETMLINTERPRETER_H
