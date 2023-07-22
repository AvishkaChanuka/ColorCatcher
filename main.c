#include "raylib.h"

int main(){

    /*
     *******************************************
     * Initialization and Load Resources
     *******************************************
    */

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth,screenHeight,"Color Catcher");
    SetTargetFPS(60);


    while(!WindowShouldClose()){

        /*
         *******************************************
         * Handle the player's input
         *******************************************
        */

        /*
         *******************************************
         * Update the scene
         *******************************************
        */


        /*
         *******************************************
         * Draw the scene
         *******************************************
        */

        BeginDrawing();
        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    /*
     *******************************************
     * DeInitialization and Unload Resources
     *******************************************
    */

    CloseWindow();

    return 0;
}