#include "raylib.h"
#include <stdlib.h>
#include <string.h>

#define MAX_SPEED 250
#define MIN_SPEED 200
#define NO_LIVES 3

const int screenWidth = 350;
const int screenHeight = 600;

//UI
int score = 0;
int lives = NO_LIVES;

bool isFirstWave = true;

Color colors[] = {RED,BLUE,GOLD,DARKGREEN};

typedef enum GameScreen {TITLE,GAMEPLAY,ENDING} GameScreen;

typedef struct Player{
    Vector2 position;
    int width, height;
    Color color;
    float speed;
    Rectangle bounds;
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
bool AreColorEqual(Color color1, Color color2);
char CheckCollision(FallingObject *object, int size, Player player);

int main(){

    /*
     *******************************************
     * Initialization and Load Resources
     *******************************************
    */

    InitWindow(screenWidth,screenHeight,"Color Catcher");
    SetTargetFPS(60);

    GameScreen currentScreen = TITLE;

    int colorIndex = 0;
    int colorSize = sizeof (colors) / sizeof (colors[0]);
    int liveBarLength;

    char scoreText[12];

    //Player Initialize
    Player player;
    player.width = 80;
    player.height = 20;
    player.position.x = (float)screenWidth/2 - (float)player.width/2;
    player.position.y = (float)(screenHeight - 50 - player.height);
    player.color = DARKGRAY;
    player.speed = 300;
    player.bounds = (Rectangle){player.position.x,player.position.y,(float)player.width,(float)player.height};

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

        switch (currentScreen) {
            case TITLE:{

                if(IsKeyPressed(KEY_ENTER)){
                    currentScreen = GAMEPLAY;
                }

            }break;
            case GAMEPLAY:{

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

            }break;
            case ENDING:{

                if(IsKeyPressed(KEY_ENTER)){
                    currentScreen = TITLE;
                }

            }break;
        }


        /*
         *******************************************
         * Update the scene
         *******************************************
        */

        switch (currentScreen) {
            case TITLE:{

            }break;
            case GAMEPLAY:{

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

                //Check Collision
                char isCollided = CheckCollision(fallingObjects,noFallingObjects,player);
                char strScore[3];

                if(isCollided == 'P'){
                    score++;
                }
                else if(isCollided == 'D'){
                    lives--;
                }

                strcpy(scoreText,"Score: ");
                itoa(score,strScore,10);
                strcat(scoreText, strScore);

                liveBarLength = (screenWidth-20) * lives / NO_LIVES;

                if(lives <=0){
                    currentScreen = ENDING;
                    lives = 3;
                    score = 0;
                }

            }break;
            case ENDING:{

            }break;
        }

        /*
         *******************************************
         * Draw the scene
         *******************************************
        */

        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentScreen) {
            case TITLE:{

                DrawText("Color Catcher",30,40,40,DARKGRAY);

                DrawText("Press Enter to Start",60,screenHeight-40,20,DARKGRAY);

            }
                break;
            case GAMEPLAY:{

                //Draw player
                DrawRectangle((int)player.position.x,(int)player.position.y,player.width,player.height,player.color);

                //DrawObjects
                DrawObjects(fallingObjects,noFallingObjects);

                DrawText(scoreText,10,10,20,DARKGRAY);

                DrawRectangle(10,40,liveBarLength,8,RED);

            }
                break;
            case ENDING:{
                DrawText("Game Over",30,30,55,DARKGRAY);
                DrawText("Color Catcher",60,120,30,DARKGRAY);

                DrawText(scoreText,screenWidth/2-70,screenHeight/2,30,DARKGRAY);

                DrawText("Press Enter to Restart",60,screenHeight-40,20,DARKGRAY);
            }
                break;

        }

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
            objects[i].speed = (float)GetRandomValue(MIN_SPEED,MAX_SPEED);

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
                lives--;
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

char CheckCollision(FallingObject *object, int size, Player player){
    bool isCollided = false;
    Rectangle playerRec;
    for(int i = 0; i<size; i++){

        if(object[i].isAlive == true && object[i].center.y > (float)screenHeight/2){

            playerRec = (Rectangle){player.position.x,player.position.y,(float)player.width,(float)player.height};
            isCollided =  CheckCollisionCircleRec(object[i].center,object[i].radius,playerRec);

            if(isCollided == true){
                object[i].isAlive = false;

                if(AreColorEqual(object[i].color,player.color)){
                    return 'P';
                }
                else{
                    return 'D';
                }
            }
        }
        else{
            continue;
        }
    }

    return 'N';
}

bool AreColorEqual(Color color1, Color color2){
    return (color1.r == color2.r && color1.g == color2.g && color1.b == color2.b && color1.a == color2.a);
}
