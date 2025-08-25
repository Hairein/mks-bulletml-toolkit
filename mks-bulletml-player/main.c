#include <stdio.h>
#include "mks-bulletml-interpreter.h"

int main()
{
    mksbmli_init();

    printf("Hello World!\n");

    mksbmli_shutdown();

    return 0;
}
