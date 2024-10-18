#include "mainMenu.h"
namespace menu {

    void mainMenu() {
        SetConfigFlags(FLAG_MSAA_4X_HINT);
        SetConfigFlags(FLAG_WINDOW_HIGHDPI);
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);

        InitWindow(1280, 720, "CryptoFi");
        SetTargetFPS(60);
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("test text", GetScreenWidth() / 2, GetScreenHeight() / 2 - 20, 20, LIGHTGRAY);
            EndDrawing();
        }
    }

}