#include "raylib.h"

int main(){

    //Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth,screenHeight,"Color Catcher");

    SetTargetFPS(60);

    while(!WindowShouldClose()){

        //Update

        //Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    //DeInitialization
    CloseWindow();

    return 0;
}