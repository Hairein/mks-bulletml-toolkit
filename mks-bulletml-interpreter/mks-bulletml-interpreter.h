#ifndef MKSBULLETMLINTERPRETER_H
#define MKSBULLETMLINTERPRETER_H

#ifdef _WIN32
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT __attribute__((visibility("default")))
#endif

#include <stdio.h>
#include <stdlib.h>

#define MKSBMLI_NO_ERROR 0

EXPORT int mksbmli_init();
EXPORT int mksbmli_shutdown();

EXPORT int mksbmli_set_random_seed(int seed);

#endif // MKSBULLETMLINTERPRETER_H
