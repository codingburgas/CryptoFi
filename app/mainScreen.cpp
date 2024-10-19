#include "mainScreen.h"

namespace mainScreen {
    void mainScreen() {
        while(!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Success", GetScreenWidth()/2, GetScreenHeight()/2, 20, GRAY);
            EndDrawing();
        }
    }
}