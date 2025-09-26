#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32) || defined(_WIN64)
  #include "dirent.h"
#else
  #include <dirent.h>
#endif
#include <sys/types.h>

#include "mksbmli-defines.h"
#include "mksbmlp-defines.h"
#include "utility.h"

void load_xml_filenames(const char* folder_path, char filenames[MKSBMLI_MAX_PLAYBACK_HANDLES][MKSBMLI_XML_FILENAME_MAX_LENGTH], int* count) {
    DIR* dir = opendir(folder_path);
    if (!dir) {
        perror("Can't open folder");
        return;
    }

    struct dirent* entry;
    *count = 0;

    while ((entry = readdir(dir)) != NULL && *count < MKSBMLI_MAX_PLAYBACK_HANDLES) {
        if (entry->d_type == DT_REG) {
            const char* name = entry->d_name;
            const char* ext = strrchr(name, '.');
            if (ext && strcmp(ext, ".xml") == 0) {
                snprintf(filenames[*count], MKSBMLI_XML_FILENAME_MAX_LENGTH, "%s/%s", folder_path, name);
                (*count)++;
            }
        }
    }

    closedir(dir);
}
