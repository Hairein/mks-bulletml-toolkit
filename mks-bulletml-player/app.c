#include "app.h"

Color bg_color = {22, 22, 22, 255};
Color playfield_bg_color = {8, 8, 8, 255};

int init_app(App* app) {
    app->virtual_playfield_dims = (Vector2){240.0f, 320.0f};
    app->projected_playfield = (Rectangle){0.0f, 0.0f, 0.0f, 0.0f};

    for(int index = 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        app->playback_handles[index] = 0;
    }

    printf("\n");
    const char test1_xml_filename[] = "/home/mkoleoso/GitHub/mks-bulletml-toolkit/bulletml_files/basic_bml.xml";
    int add_result1 = add_xml_file(app, test1_xml_filename);
    printf("add xml file %s result: %d\n\n", test1_xml_filename, (int)add_result1);

    printf("\n");
    const char test2_xml_filename[] = "/home/mkoleoso/GitHub/mks-bulletml-toolkit/bulletml_files/inter_bml.xml";
    int add_result2 = add_xml_file(app, test2_xml_filename);
    printf("add xml file %s result: %d\n\n", test2_xml_filename, (int)add_result2);

    printf("\n");
    const char test3_xml_filename[] = "/home/mkoleoso/GitHub/mks-bulletml-toolkit/bulletml_files/aimed_bml.xml";
    int add_result3 = add_xml_file(app, test3_xml_filename);
    printf("add xml file %s result: %d\n\n", test3_xml_filename, (int)add_result3);

    return MKSBMLP_NO_ERROR;
}

int update_app(App* app) {
    return MKSBMLP_NO_ERROR;
}

int render_app(App* app) {
    calculate_playfield(app);

    ClearBackground(bg_color);

    DrawRectangle(app->projected_playfield.x, app->projected_playfield.y, app->projected_playfield.width, app->projected_playfield.height, playfield_bg_color);

    return MKSBMLP_NO_ERROR;
}

int set_playfield_dims(App* app, int width, int height)
{
    if(width < 64 || width > 4096 || height < 64 || height > 4096) {
        return MKSBMLP_INVALID_PARAMETER;
    }

    app->virtual_playfield_dims = (Vector2){width, height};

    return MKSBMLP_NO_ERROR;
}

int add_xml_file(App* app, const char* xml_filename){
    MKSBMLI_PLAYBACK_HANDLE handle;
    int load_result = mksbmli_load_xml(xml_filename, &handle);
    if(load_result != MKSBMLP_NO_ERROR) return load_result;
    if(handle == 0) return MKSBMLP_UNSPECIFIED_ERROR;

    for(int index = 0; index < MKSBMLI_MAX_PLAYBACK_HANDLES; index++) {
        if(app->playback_handles[index] != 0) continue;

        strcpy((char*)&app->xml_filenames[index], xml_filename);
        app->playback_handles[index] = handle;

        return MKSBMLP_NO_ERROR;
    }

    return MKSBMLP_TOO_MANY_XML_FILES;
}

void calculate_playfield(App* app) {
    float screen_width = GetScreenWidth();
    float screen_height = GetScreenHeight();

    float virtual_width_ratio = app->virtual_playfield_dims.x / app->virtual_playfield_dims.y;

    float virtual_width = screen_width;
    float virtual_height = screen_width / virtual_width_ratio;

    if(virtual_height > screen_height) {
        float virtual_height_ratio = app->virtual_playfield_dims.y / screen_height;
        virtual_width = app->virtual_playfield_dims.x / virtual_height_ratio;
        virtual_height = screen_height;
    }

    app->projected_playfield.x = screen_width / 2.0f - (virtual_width / 2.0f);
    app->projected_playfield.y = screen_height / 2.0f - (virtual_height / 2.0f);
    app->projected_playfield.width = virtual_width;
    app->projected_playfield.height = virtual_height;
}
