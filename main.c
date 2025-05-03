#include <stdio.h>
#include "raylib.h"
#include <time.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define MAX_PIPES 6
#define NUM_SPRITES 3

void pipesLogic();
void birdColision();
void updateSuperiorPipes();
void resetPipesPosition();
void gameplayLogic();
void resetGameplay();
void jump();

void gameOverScreen();

Rectangle Bird = {200,200, 60, 60};

short jumpForce = 350;
short velocity = 0;
float gravity = 900;
float birdRotation = 0.0f;
float gap = 200;
int score = 0;

Texture2D BirdSpries[NUM_SPRITES];



Rectangle Pipes[MAX_PIPES];

Rectangle Pipe = {420,400, 100, 1000};
Rectangle Pipe2 = {852, 400, 100, 1000};
Rectangle Pipe3 = {1278, 400, 100, 1000};

Rectangle Pipe4 = {0,0, 100, 1000};
Rectangle Pipe5 = {0,0, 100, 1000};
Rectangle Pipe6 = {0,0, 100, 1000};


bool pause = true;

bool firstGame = true;

char currentText[10];

int main() {
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird fan game");
    
    SetTargetFPS(60);
    InitAudioDevice();
    

    SetRandomSeed((unsigned int)time(NULL));

    Image icon = LoadImage("assets/bird.png");
    SetWindowIcon(icon);


    Sound loseSound = LoadSound("assets/Pou game over sound effect.ogg");
    Music music = LoadMusicStream("assets/Pou music ConnectCliff JumpCliff DashJetmp3.ogg");

    Texture2D background = LoadTexture("assets/background_Trasparent.png");

    Texture2D inferiorPipe = LoadTexture("assets/inferior pipe.png");
    Texture2D superiorPipe = LoadTexture("assets/superior pipe.png");

    Texture2D bird1 = LoadTexture("assets/bird1.png");
    Texture2D bird2 = LoadTexture("assets/bird2.png");
    Texture2D bird3 = LoadTexture("assets/bird3.png");


    
    
    BirdSpries[0] = bird1;
    BirdSpries[1] = bird2;
    BirdSpries[2] = bird3;
    
    

    Pipes[0] = Pipe;
    Pipes[1] = Pipe2;
    Pipes[2] = Pipe3;

    Pipes[3] = Pipe4;
    Pipes[4] = Pipe5;
    Pipes[5] = Pipe6;

    
    for (int i = 0; i < sizeof(Pipes) / sizeof(Pipes[0]); i++) {
        Pipes[i].y = GetRandomValue(200,700);
        
    }
    
    updateSuperiorPipes();


    while (!WindowShouldClose()) {
        

        BeginDrawing();
        ClearBackground(RAYWHITE);


        if (!pause) {
            ClearBackground(SKYBLUE);

            UpdateMusicStream(music);

        if (!IsMusicStreamPlaying(music)) {
            PlayMusicStream(music);
        }
            PlaySound(loseSound);

            float deltaTime = GetFrameTime();
                
            
            if (IsKeyPressed(KEY_R)) resetGameplay();
        
            if (IsKeyPressed(KEY_SPACE)) jump();

            pipesLogic();

            birdColision();

            if (birdRotation < 20) birdRotation += 0.8;
        
            //forces
            velocity += gravity * deltaTime;
            Bird.y += velocity *  deltaTime;
        


            //DrawTextureEx(background,(Vector2){ 0, 0},0.0F, 2.6 , WHITE);
            //DrawTextureRec(background,(Rectangle){0,0,background.width, background.height}, (Vector2){0,0},WHITE);
            DrawTexturePro(background, (Rectangle){0,0, background.width, background.height}, (Rectangle){0,0, SCREEN_WIDTH, SCREEN_HEIGHT}, (Vector2){0,0},0.0, WHITE);

            //DrawRectangleRec(Bird, YELLOW);
            DrawTextureEx(bird1, (Vector2){Bird.x -8, Bird.y - 16}, birdRotation, 4.5, WHITE);
            
            
            for (int i = 0; i < sizeof(Pipes) / sizeof(Pipes[0]); i++) {
                //DrawRectangleRec(Pipes[i], GREEN);   
                
            }
            for (int i = 0; i < 3; i++) {
                DrawTextureEx(inferiorPipe, (Vector2){Pipes[i].x-3 , Pipes[i].y-4}, 0.0f, 4, WHITE);
                
            }
            for (int i = 3; i < sizeof(Pipes) / sizeof(Pipes[0]); i++) {
                DrawTextureEx(superiorPipe, (Vector2){Pipes[i].x-10 , Pipes[i].y + superiorPipe.height + 180 }, 0.0f, 4, WHITE);
                
            }



        }

        else {
            
            StopMusicStream(music);
            gameOverScreen();
            
        }

        

        EndDrawing();
    }
    
    for (int i = 0; i < sizeof(BirdSpries) / sizeof(BirdSpries[0]); i++) {
        UnloadTexture(BirdSpries[i]);
    }
    UnloadTexture(inferiorPipe);
    UnloadTexture(superiorPipe);
    UnloadTexture(background);
    UnloadSound(loseSound);
    UnloadMusicStream(music);
    
    CloseWindow();
    
    return 0;
}

void resetGameplay() {
    birdRotation = 0;
    score = 0;
    Bird.y = 200;
    velocity = 0 ;
    resetPipesPosition();
}
void jump() {
    velocity = 0;
    velocity -= jumpForce;
    birdRotation = -20;
}



void gameOverScreen() {
    if (firstGame) {
        DrawText("flappy bird", 350, 100,100, BLACK);
    }
    else {
        DrawText("Game Over", 350, 100,100, BLACK);
        DrawText(TextFormat("Score: %d", score), 500, 220, 50, BLACK);    
    }
    if (GuiButton((Rectangle){470, 300,300,150},"") || IsKeyPressed(KEY_ENTER)) {
        pause = false;
        resetGameplay();
    }    
    DrawText("Play", 500, 320, 100, DARKGRAY);
}


void pipesLogic() {

    for (int i = 0; i < sizeof(Pipes) / sizeof(Pipes[0]); i++ ) {
        Pipes[i].x -= 200 * GetFrameTime();
    }

    //verifica se o cano saiu da tela
    for (int i = 0; i < sizeof(Pipes) / sizeof(Pipes[0]); i++ ) {
        if (Pipes[i].x + Pipes[i].width < 0) {
            
            Pipes[i].x = SCREEN_WIDTH;
            Pipes[i].y = GetRandomValue(200, 600);
            printf("Pipe[%d].y = %0.0f\n", i, Pipes[i].y);
            
            score++;

            updateSuperiorPipes();   
        }   
    }
    
}
void updateSuperiorPipes() {
    Pipes[3].x = Pipes[0].x;
    Pipes[3].y = Pipes[0].y - gap - Pipes[3].height;
    
    Pipes[4].x = Pipes[1].x;
    Pipes[4].y = Pipes[1].y - gap - Pipes[4].height;

    Pipes[5].x = Pipes[2].x;
    Pipes[5].y = Pipes[2].y - gap - Pipes[5].height;
}



void birdColision() {
    for (int i = 0; i < sizeof(Pipes) / sizeof(Pipes[0]); i++) {
        if (Bird.x + Bird.width > Pipes[i].x && Bird.x < Pipes[i].x + Pipes[i].width &&
            Bird.y + Bird.height > Pipes[i].y && Bird.y < Pipes[i].y + Pipes[i].height) {

            firstGame = false;
            pause = true;
            

        }

    }
}

void resetPipesPosition() {
    Pipes[0].x = 400;
    Pipes[1].x = 800;
    Pipes[2].x = 1200;

    Pipes[0].y = GetRandomValue(200, 600);
    Pipes[1].y = GetRandomValue(200, 600);
    Pipes[2].y = GetRandomValue(200, 600);

    updateSuperiorPipes();
}