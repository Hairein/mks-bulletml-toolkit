#ifndef UTILITY_H
#define UTILITY_H

#include "app.h"

void set_playfield_dims(App* app, int width, int height);
int add_xml_file(App* app, const char* xml_filename);
void calculate_playfield(App* app);
void load_xml_filenames(const char* folder_path, char filenames[MKSBMLI_MAX_PLAYBACK_HANDLES][MKSBMLI_XML_FILENAME_MAX_LENGTH], int* count);

#endif // UTILITY_H
