#ifndef UTILITY_H
#define UTILITY_H

#include "app.h"

void set_playfield_dims(App* app, int width, int height);
int add_xml_file(App* app, const char* xml_filename);
void calculate_playfield(App* app);

#endif // UTILITY_H
