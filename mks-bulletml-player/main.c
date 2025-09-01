#include <stdlib.h>
#include <stdio.h>

#include "mks-bulletml-interpreter.h"
#include "app.h"

int main()
{
    char title[MKSBMLI_MAX_TEXT_LENGTH];
    sprintf(title, "MKS BulletML Player v%d.%d.%d startup.", MKSBMLP_MAJOR_VERSION, MKSBMLP_MINOR_VERSION, MKSBMLP_PATCH_VERSION);

    mksbmli_init();

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, title);
    SetTargetFPS(60);
    SetWindowMinSize(64, 64);

    App* app = calloc(1, sizeof(App));
    init_app(app);

    while (!WindowShouldClose())
    {
        update_app(app);

        BeginDrawing();
        render_app(app);
        EndDrawing();

        post_update_app(app);
    }

    CloseWindow();

    free(app);

    mksbmli_shutdown();

    return 0;
}
