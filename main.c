#include "raylib.h"

/*
 *******************************************
 * typedefs and structures
 *******************************************
*/

//Player
typedef struct Player{
    Vector2 position;
    int width, height;
    Color color;
    float speed;
}Player;

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

    //Player
    Player player;
    player.width = 80;
    player.height = 20;
    player.position.x = (float)screenWidth/2 - (float)player.width/2;
    player.position.y = (float)(screenHeight - 50 - player.height);
    player.color = DARKGRAY;
    player.speed = 300;


    while(!WindowShouldClose()){

        float deltaTime = GetFrameTime();

        /*
         *******************************************
         * Handle the player's input
         *******************************************
        */

        if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)){
            player.position.x -= player.speed * deltaTime;
        }

        if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)){
            player.position.x += player.speed * deltaTime;
        }

        /*
         *******************************************
         * Update the scene
         *******************************************
        */

        //Set player in the screen boundary
        if(player.position.x < 0){
            player.position.x = 0;
        }

        float maxPlayerX = (float)screenWidth-(float)player.width;
        if(player.position.x > maxPlayerX){
            player.position.x = maxPlayerX;
        }

        /*
         *******************************************
         * Draw the scene
         *******************************************
        */

        BeginDrawing();
        ClearBackground(RAYWHITE);

        //Draw player
        DrawRectangle((int)player.position.x,(int)player.position.y,player.width,player.height,player.color);

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