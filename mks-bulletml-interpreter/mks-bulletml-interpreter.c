#include <libxml/parser.h>
#include <libxml/tree.h>

#include "platform_random.h"

#include "mks-bulletml-interpreter.h"

int mksbmli_init()
{
    printf("MKS BulletML Interpreter Library v0.0.1 startup.\n");

    mksbmli_set_random_seed(1337);

    return MKSBMLI_NO_ERROR;
}

int mksbmli_shutdown()
{
    printf("MKS BulletML Interpreter Library shutdown.\n");

    return MKSBMLI_NO_ERROR;
}

int mksbmli_set_random_seed(int seed)
{
    INIT_RANDOM(seed);

    return MKSBMLI_NO_ERROR;
}
