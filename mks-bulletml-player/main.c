#include <stdlib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "mks-bulletml-interpreter.h"
#include "app.h"

int main()
{
    mksbmli_init();

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "MKS BulletML Player v0.0.1preAlpha");
    SetTargetFPS(60);
    SetWindowMinSize(64, 64);

    App* app = calloc(1, sizeof(App));
    init_app(app);
    set_playfield_dims(app, 240, 320);

    while (!WindowShouldClose())
    {
        int quit_request = update_app(app);

        BeginDrawing();
        render_app(app);
        EndDrawing();

        if(quit_request != MKSBMLP_NO_ERROR) break;
    }

    CloseWindow();

    free(app);

    mksbmli_shutdown();

    return 0;
}
