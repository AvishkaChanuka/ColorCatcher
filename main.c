#include "raylib.h"

const int screenWidth = 350;
const int screenHeight = 600;

bool isFirstWave = true;

Color colors[] = {RED,BLUE,GOLD,DARKGREEN};

typedef struct Player{
    Vector2 position;
    int width, height;
    Color color;
    float speed;
} Player;

typedef struct FallingObject{
    Vector2 center;
    float radius;
    int sides;
    float rotation;
    Color color;
    float speed;
    bool isAlive;
} FallingObject;

Color RandomColor();
void ObjectBehaviour(FallingObject *objects, int size, float deltaTime);
void DrawObjects(FallingObject *objects, int size);

int main(){

    /*
     *******************************************
     * Initialization and Load Resources
     *******************************************
    */

    InitWindow(screenWidth,screenHeight,"Color Catcher");
    SetTargetFPS(60);

    int colorIndex = 0;
    int colorSize = sizeof (colors) / sizeof (colors[0]);

    //Player Initialize
    Player player;
    player.width = 80;
    player.height = 20;
    player.position.x = (float)screenWidth/2 - (float)player.width/2;
    player.position.y = (float)(screenHeight - 50 - player.height);
    player.color = DARKGRAY;
    player.speed = 300;

    //Falling  Objects
    int noFallingObjects = 6;
    FallingObject fallingObjects[noFallingObjects];

    while(!WindowShouldClose()){

        float deltaTime = GetFrameTime();

        /*
         *******************************************
         * Handle the player's input
         *******************************************
        */

        //Player Movement
        if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)){
            player.position.x -= player.speed * deltaTime;
        }

        if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)){
            player.position.x += player.speed * deltaTime;
        }

        //Change Color
        if(IsKeyPressed(KEY_SPACE)){

            player.color = colors[colorIndex];
            colorIndex++;
            if(colorIndex == colorSize){
                colorIndex = 0;
            }
        }

        /*
         *******************************************
         * Update the scene
         *******************************************
        */

        //Set the player in the screen boundary
        if(player.position.x < 0){
            player.position.x = 0;
        }

        float maxPlayerX = (float)screenWidth-(float)player.width;
        if(player.position.x > maxPlayerX){
            player.position.x = maxPlayerX;
        }

        //Setup Falling Objects
        ObjectBehaviour(fallingObjects,noFallingObjects, deltaTime);

        /*
         *******************************************
         * Draw the scene
         *******************************************
        */

        BeginDrawing();
        ClearBackground(RAYWHITE);

        //Draw player
        DrawRectangle((int)player.position.x,(int)player.position.y,player.width,player.height,player.color);

        //DrawObjects
        DrawObjects(fallingObjects,noFallingObjects);

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

void ObjectBehaviour(FallingObject *objects, int size, float deltaTime){

    for(int i = 0; i<size; i++){

        if(objects[i].isAlive == false){
            //Initialize object
            objects[i].radius = 20;

            objects[i].center.x = (float)GetRandomValue((int)(objects[i].radius * 2),(screenWidth - 2 * (int)objects[i].radius));
            objects[i].center.y = -2 * objects[i].radius;
            //objects[i].center.y = (float)GetRandomValue(-screenWidth,0);

            objects[i].sides = 4;
            objects[i].rotation = 45;
            objects[i].speed = (float)GetRandomValue(100,120);

            objects[i].color = RandomColor();

            if(isFirstWave == true){
                objects[i].isAlive = true;
                isFirstWave = false;
            }
        }
        else{
            //Move Object
            objects[i].center.y += objects[i].speed * deltaTime;

            if(objects[i].center.y > (float)(screenHeight)/2){
                if(i == size - 1){
                    objects[0].isAlive = true;
                }
                else{
                    objects[i+1].isAlive = true;
                }
            }

            if(objects[i].center.y > (float)(screenHeight) + 2 * objects[i].radius){
                objects[i].isAlive = false;
            }
        }
    }

}

void DrawObjects(FallingObject *objects, int size){

    for(int i = 0; i<size; i++){
        if(objects[i].isAlive){
            DrawPoly(objects[i].center, objects[i].sides, objects[i].radius, objects[i].rotation, objects[i].color);
        }
    }
}

Color RandomColor(){
    int size = sizeof(colors) / sizeof(colors[0]);
    int rand = GetRandomValue(0,size-1);
    return colors[rand];
}