#ifndef MKSBULLETMLINTERPRETER_H
#define MKSBULLETMLINTERPRETER_H

#ifdef _WIN32
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT __attribute__((visibility("default")))
#endif

#include <stdio.h>

EXPORT int mksbmli_init();
EXPORT int mksbmli_shutdown();

#endif // MKSBULLETMLINTERPRETER_H
