#ifndef PLATFORM_RANDOM_H
#define PLATFORM_RANDOM_H

#ifdef _WIN32
    #include <stdlib.h>
    #define INIT_RANDOM(seed) srand(seed)
    #define GET_RANDOM() rand()
#else
    #include <stdlib.h>
    #define INIT_RANDOM(seed) srandom(seed)
    #define GET_RANDOM() random()
#endif

#endif // PLATFORM_RANDOM_H
