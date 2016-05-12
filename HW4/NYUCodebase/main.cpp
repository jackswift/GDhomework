#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include <vector>
#include "Bullet.h"
#include "LoadAndDrawFuncs.h"
#include "Entity.h"
#include "MixerSound.h"
#include <fstream>
#include <iostream>
#include <sstream> 
#include <string> 
#include "levelData.h"
#include "initializiationStuff.h"
#include "enemyInteractions.h"
#include "SDL_mixer.h"


using namespace std;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define SPRITE_COUNT_X 30
#define SPRITE_COUNT_Y 30
#define TILE_SIZE 0.17f
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define NUM_OF_SOLID_TILES 9

float MaxYPos = 2.0f;
float MaxXPos = 3.55f;
bool done = false;
int mapWidth = -1;
int mapHeight = -1;
int solidTiles[NUM_OF_SOLID_TILES] = {121, 128, 38, 154, 43, 152, 8, 4, 32};

bool player1Alive = true;
float timeDead = 0.0f;
int currentLevel = 1;

const int numFrames = 5;
float animationElapsed = 0.0f;
float framesPerSecond = 30.0f;
int currentIndex = 1;
float timeUntilNextLevel = 0.0f;

//Gamestate enum and state
enum GameState {STATE_MAIN_MENU, STATE_GAME_LEVEL1, INGAME_MENU};
int state;

const Uint8 *keys = SDL_GetKeyboardState(NULL);

SDL_Window* displayWindow;


//SETUP AND EVENTS FUNCTIONS
void setBackgroundColorAndClear();
void Setup(Matrix &projectionMatrix);
void ProcessEvents();

//UPDATE FUNCTIONS
void Update(float elapsed, Matrix &modelMatrix, ShaderProgram program, Matrix &viewMatrix);
void UpdateGameLevel(float elapsed, Matrix &modelMatrix, ShaderProgram program, Matrix &viewMatrix);
void tileCollisionBot(float elapsed, Matrix &modelMatrix, ShaderProgram program, int index);
void tileCollisionTop(float elapsed, Matrix &modelMatrix, ShaderProgram program, int index);
void tileCollisionLeft(float elapsed, Matrix &modelMatrix, ShaderProgram program, int index);
void tileCollisionRight(float elapsed, Matrix &modelMatrix, ShaderProgram program, int index);

//RENDER FUNCTIONS
void Render(ShaderProgram program, Matrix modelMatrix);
void RenderGameLevel(ShaderProgram program, Matrix modelMatrix);
void RenderMainMenu(ShaderProgram program, Matrix modelMatrix);
void renderTileLevel(ShaderProgram *program);
void setViewMatrix(ShaderProgram program, Matrix viewMatrix);

//INITIALIZATION OF ENTITY VECTOR
//void initializeEntities();
//void placeEntity(string type, float xPosition, float yPosition);

void progressToNextLevel(float elapsed);
void renderInGameMenu(ShaderProgram program, Matrix modelMatrix);

//Tile stuff
void readData(string file, int level);
bool readEntityData(ifstream &stream);
bool readLayerData(ifstream &stream, int level);
bool readHeader(ifstream &stream, int level);

void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY);

//COLLISION DETECTION (MIGHT GO INTO ENTITY FILE)
//bool DetectCollision(Entity entityOne, Entity entityTwo);
//bool DetectCollisionBullet(Entity entityOne/* paddle */, Bullet bullet /*bullet*/);
bool DetectCollisionBullet(Entity entityOne/* paddle */, Bullet bullet /*pongBall*/);

//Vector of entities
std::vector<Entity> vectorOfEnts;
std::vector<Bullet> bullets;

//Sound files
Mix_Chunk *jumpSound;
Mix_Chunk *hurtSound;
Mix_Chunk *icicleSound;
Mix_Chunk *endOfLevel;

//Num samples for sounds
unsigned int numSamples = 0;

int main(int argc, char *argv[])
{
    
    //state = STATE_MAIN_MENU;
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    Setup(projectionMatrix);
    glDisable(GL_CULL_FACE);
    float ticks;
    float lastFrameTicks = 0.0f;
    float elapsed;
    
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    state = STATE_MAIN_MENU;
  
    /* SOUND STUFF*/
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    Mix_Music *introSong;
    introSong = Mix_LoadMUS("Intro_Song.mp3");
    
    
    //Begin playing music
    Mix_PlayMusic(introSong, -1);
    //enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    readData("/Users/lyletrue/MyGameDesignHW2/HW4/NYUCodebase/stage1Tiles.txt", 1);
    readData("/Users/lyletrue/MyGameDesignHW2/HW4/NYUCodebase/level2.txt", 2);
    readData("/Users/lyletrue/MyGameDesignHW2/HW4/NYUCodebase/stage3Tiles.txt", 3);
    
    while (!done) {
        
        ticks = (float)SDL_GetTicks() / 1000.0f;
        elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        ProcessEvents();
        
        glUseProgram(program.programID);
        //p1Entity.Update(elapsed);
        setBackgroundColorAndClear();
        program.setModelMatrix(modelMatrix);
        program.setViewMatrix(viewMatrix);
        program.setProjectionMatrix(projectionMatrix);
        //cout << state << endl;
        //cout << "why this no happen main menu?" << endl;
        float fixedElapsed = elapsed;
        if(fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
            fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
        }
        while (fixedElapsed >= FIXED_TIMESTEP ) {
            fixedElapsed -= FIXED_TIMESTEP;
            Update(FIXED_TIMESTEP, modelMatrix, program, viewMatrix);
        }
        Update(fixedElapsed, modelMatrix, program, viewMatrix);
        
        //setViewMatrix(program, viewMatrix);
        
        Render(program, modelMatrix);
        
        SDL_GL_SwapWindow(displayWindow);
    }
    Mix_FreeMusic(introSong);
    Mix_FreeChunk(hurtSound);
    Mix_FreeChunk(icicleSound);
    Mix_FreeChunk(jumpSound);
    Mix_FreeChunk(endOfLevel);
    SDL_Quit();
    return 0;
}

void Setup(Matrix& projectionMatrix)
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640*1.5, 360*1.5, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
    //Program and window stuff
    glViewport(0, 0, 640*1.5, 360*1.5);
    
}

void ProcessEvents()
{
    jumpSound = Mix_LoadWAV("jumpSound.wav");
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if(state == STATE_GAME_LEVEL1)
            {
                if(event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                {
                    //only allows jumping when the entity is touching the ground
                    if(vectorOfEnts[0].collidedBot)
                    {
                        Mix_PlayChannel(-1, jumpSound, 0);
                        vectorOfEnts[0].yVelocity = 1.8f;
                    }
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_M)
                {
                    state = INGAME_MENU;
                    //sets state to INGAME_MENU
                    //cout << "Does this happen?!" << endl;
                    
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_E)
                {
                    cout << "x: " <<  vectorOfEnts[0].entityVector.x << " y: " << vectorOfEnts[0].entityVector.y << endl;
                }
            }
            if(state == STATE_MAIN_MENU)
            {
                if(event.key.keysym.scancode == SDL_SCANCODE_1)
                {
                    state = STATE_GAME_LEVEL1;
                    initializeEntitiesLevel1(vectorOfEnts);
                }
            }
            if(state == INGAME_MENU)
            {
                if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    state = STATE_GAME_LEVEL1;
                }
                if(event.key.keysym.scancode == SDL_SCANCODE_X)
                {
                    done = true;
                }
                
            }
        }
    }
}
void progressToNextLevel(float elapsed)
{
    //cout << "You made it to the next level!" << endl;
    timeUntilNextLevel += elapsed;
    
    if(timeUntilNextLevel > 1.5f)
    {
        currentLevel++;
        if(currentLevel == 2)
        {
            initializeEntitiesLevel2(vectorOfEnts);
        }
        if(currentLevel == 3)
        {
            initializeEntitiesLevel1(vectorOfEnts);
        }
        timeUntilNextLevel = 0.0f;
    }
    
}
void Update(float elapsed, Matrix &modelMatrix, ShaderProgram program, Matrix &viewMatrix)
{
    switch(state)
    {
        case STATE_GAME_LEVEL1:
            UpdateGameLevel(elapsed, modelMatrix, program, viewMatrix);
            break;
        case STATE_MAIN_MENU:
            //UpdateMainMenu(elapsed, modelMatrix, program);
            break;
    }
}
void collisionY(float elapsed, int index)
{
    hurtSound = Mix_LoadWAV("hurtSound.wav");
    if(index != 0)
    {
        if(vectorOfEnts[index].entityType == ENTITY_PLATFORM)
        {
            if(vectorOfEnts[0].collidesWith(&vectorOfEnts[index]))
            {
                if(vectorOfEnts[0].entityVector.y > vectorOfEnts[index].entityVector.y)
                {
                    float valueOfActualPlatform = vectorOfEnts[index].entityVector.y - vectorOfEnts[index].height/2 + 0.057;
                    float penetration = fabsf((vectorOfEnts[0].entityVector.y - valueOfActualPlatform)-vectorOfEnts[0].height/2);
                    vectorOfEnts[0].entityVector.y += penetration + 0.00001f;
                    vectorOfEnts[0].collidedBot = true;
                    vectorOfEnts[0].yVelocity = 0.0f;
                    vectorOfEnts[0].xVelocity = vectorOfEnts[index].xVelocity;
                }
                else if(vectorOfEnts[0].entityVector.y < vectorOfEnts[index].entityVector.y)
                {
                    //vectorOfEnts[index].entityVector.y + (vectorOfEnts[index].height/2)
                    //- ((-TILE_SIZE * yTilePosTop) - TILE_SIZE +0.0000001f);
                    float valueToUse = vectorOfEnts[index].entityVector.y - vectorOfEnts[index].height/2;
                    float penetration = fabsf((vectorOfEnts[0].entityVector.y - (valueToUse-0.057))+vectorOfEnts[0].height/2);
                    vectorOfEnts[0].entityVector.y -= penetration + 0.00001f;
                    vectorOfEnts[0].collidedTop = true;
                }
            }
        }
        if(vectorOfEnts[index].entityType == ENTITY_FIST)
        {
            if(vectorOfEnts[0].collidesWith(&vectorOfEnts[index]))
            {
                //if entity is below threshold, dies, if above threshold, lives and stands on top of
                //fist.
                float botOfFist = vectorOfEnts[index].entityVector.y - vectorOfEnts[index].height/4;
                //float botOfEnt = vectorOfEnts[0].entityVector.y - vectorOfEnts[0].height/2;
                float topOfFist = vectorOfEnts[index].entityVector.y + vectorOfEnts[index].height/2;
                if(vectorOfEnts[0].entityVector.y > botOfFist)
                {
                    float penetration = fabsf((vectorOfEnts[0].entityVector.y - (topOfFist))-vectorOfEnts[0].height/2);
                    vectorOfEnts[0].entityVector.y += penetration + 0.0000001f;
                    vectorOfEnts[0].collidedBot = true;
                    vectorOfEnts[0].yVelocity = 0.0f;
                }
                else if(vectorOfEnts[0].entityVector.y < botOfFist)
                {
                    Mix_PlayChannel(-1, hurtSound, 0);
                    player1Alive = false;
                
                }
                
            }
        }
    }
}
void collisionX(float elapsed, int index)
{
    if(index != 0)
    {
        if(vectorOfEnts[index].entityType == ENTITY_PLATFORM)
        {
            
        }
    }
}
void UpdateGameLevel(float elapsed, Matrix &modelMatrix, ShaderProgram program, Matrix &viewMatrix)
{
    setViewMatrix(program, viewMatrix);
    int animationIndex[5] = {19, 26, 27, 28, 29};
    if(keys[SDL_SCANCODE_D])
    {
        animationElapsed += elapsed;
        vectorOfEnts[0].acceleration_x = 4.0f;
        vectorOfEnts[0].rotation = 1.0f;
        if(vectorOfEnts[0].xVelocity > 0.01f)
        {
            if(animationElapsed > 1.0/framesPerSecond)
            {
                currentIndex++;
                animationElapsed = 0.0f;
                if(currentIndex > numFrames-1)
                {
                    currentIndex = 1;
                }
            }
            vectorOfEnts[0].setSprite(SPRITE_COUNT_Y, SPRITE_COUNT_X, animationIndex[currentIndex], 0.17f);
        }
        else if (vectorOfEnts[0].xVelocity == 0.0f)
        {
            vectorOfEnts[0].setSprite(SPRITE_COUNT_Y, SPRITE_COUNT_X, animationIndex[0], 0.17f);
        }
    }
    else if(keys[SDL_SCANCODE_A])
    {
        //float timeDead;
        animationElapsed += elapsed;
        vectorOfEnts[0].acceleration_x = -4.0f;
        vectorOfEnts[0].rotation = -1.0f;
        if(vectorOfEnts[0].xVelocity < -0.01f)
        {
            if(animationElapsed > 1.0/framesPerSecond)
            {
                currentIndex++;
                animationElapsed = 0.0f;
                if(currentIndex > numFrames-1)
                {
                    currentIndex = 1;
                }
            }
            vectorOfEnts[0].setSprite(SPRITE_COUNT_Y, SPRITE_COUNT_X, animationIndex[currentIndex], 0.17f);
        }
        else if (vectorOfEnts[0].xVelocity == 0.0f)
        {
            vectorOfEnts[0].setSprite(SPRITE_COUNT_Y, SPRITE_COUNT_X, animationIndex[0], 0.17f);
        }
        //vectorOfEnts[0].rotation = -1.0f;
    }
    /*
    if(vectorOfEnts[0].acceleration_x <= 0.0f)
    {
        vectorOfEnts[0].setSprite(SPRITE_COUNT_Y, SPRITE_COUNT_X, animationIndex[0], 0.17f);
    }
     */
    for(int i = 0; i < vectorOfEnts.size(); i++)
    {
        if(!vectorOfEnts[i].isStatic)
        {
            vectorOfEnts[i].collidedBot = false;
            vectorOfEnts[i].collidedTop = false;
            vectorOfEnts[i].collidedLeft = false;
            vectorOfEnts[i].collidedRight = false;
            vectorOfEnts[i].UpdateY(elapsed);
            tileCollisionBot(elapsed, modelMatrix, program, i);
            tileCollisionTop(elapsed, modelMatrix, program, i);
            collisionY(elapsed, i);
            vectorOfEnts[i].UpdateX(elapsed);
            tileCollisionLeft(elapsed, modelMatrix, program, i);
            tileCollisionRight(elapsed, modelMatrix, program, i);
        }
    }
    for(int i = 1; i < vectorOfEnts.size(); i++)
    {
        if(vectorOfEnts[i].entityType == ENTITY_ICICLE)
        {
            float distanceBetween = vectorOfEnts[0].entityVector.x - vectorOfEnts[i].entityVector.x;
            if(vectorOfEnts[0].entityVector.y < vectorOfEnts[i].entityVector.y)
            {
                if(distanceBetween < 0.1)
                {
                    if((vectorOfEnts[0].entityVector.x+vectorOfEnts[0].width/2) - (vectorOfEnts[i].entityVector.x-vectorOfEnts[i].width/2-0.05) > 0)
                    {
                        icicleFalling(vectorOfEnts[i]);
                    }
                }
                else if(distanceBetween > 0.1)
                {
                    if((vectorOfEnts[0].entityVector.x-vectorOfEnts[0].width/2) - (vectorOfEnts[i].entityVector.x+vectorOfEnts[i].width/2) < 0)
                    {
                        icicleFalling(vectorOfEnts[i]);
                    }
                }
            }
            if(vectorOfEnts[0].collidesWith(&vectorOfEnts[i]))
            {
                //icicleSound = Mix_LoadWAV("icicleFalling.wav");
                //Mix_PlayChannel(-1, icicleSound, 0);
                hurtSound = Mix_LoadWAV("hurtSound.wav");
                Mix_PlayChannel(-1, hurtSound, 0);
                player1Alive = false;
            }
        }
        else if(vectorOfEnts[i].entityType == ENTITY_FIST)
        {
            float distanceBetweenX = vectorOfEnts[0].entityVector.x - vectorOfEnts[i].entityVector.x;
            float distanceBetweenY = vectorOfEnts[0].entityVector.y - vectorOfEnts[i].entityVector.y;
            distanceBetweenY = -distanceBetweenY;
            
            //mock distance between first entity and fist = 3.485 - 2.972 =    0.513
            if(vectorOfEnts[i].waiting)
            {
                //cout << "Hey look!" << endl;
                if(distanceBetweenY < 0.6f && distanceBetweenY > 0.1f)
                {
                    //cout << "Hey look!" << endl;
                    if(distanceBetweenX < 0.01)
                    {
                        if((vectorOfEnts[0].entityVector.x+vectorOfEnts[0].width/2) - (vectorOfEnts[i].entityVector.x-vectorOfEnts[i].width/2) > 0)
                        {
                            //cout << "does this happen?" << endl;
                            fistFalling(vectorOfEnts[i]); //take care of setting appropriate bools here
                        }
                    }
                    else if(distanceBetweenX > 0.01)
                    {
                        if((vectorOfEnts[0].entityVector.x-vectorOfEnts[0].width/2) - (vectorOfEnts[i].entityVector.x+vectorOfEnts[i].width/2+0.1) < 0)
                        {
                            //cout << "does this happen?" << endl;
                            fistFalling(vectorOfEnts[i]);
                        }
                    }
                }
            }
            else if(vectorOfEnts[i].fallen)
            {
                vectorOfEnts[i].timeFallen += elapsed;
                //cout << vectorOfEnts[i].timeFallen << endl;
                if(vectorOfEnts[i].timeFallen > 1.3)
                {
                    fistReturning(vectorOfEnts[i]);
                }
            }
            else if(vectorOfEnts[i].returning)
            {
                //don't think i do anything here...
                //cout << "Still returning..." << endl;
                if(vectorOfEnts[i].entityVector.y > vectorOfEnts[i].startingPosition)
                {
                    //cout << "does this happen?" << endl;
                    vectorOfEnts[i].waiting = true;
                    vectorOfEnts[i].returning = false;
                    vectorOfEnts[i].falling = false;
                    vectorOfEnts[i].fallen = false;
                    vectorOfEnts[i].yVelocity = 0.0f;
                }
            }
            /*
            if(vectorOfEnts[0].collidesWith(&vectorOfEnts[i]) == 1)
            {
               
            }
             */
        }
        else if(vectorOfEnts[i].entityType == ENTITY_PLATFORM)
        {
            if(vectorOfEnts[i].falling == true)
            {
                vectorOfEnts[i].xVelocity = -0.8f;
            }
            else if(vectorOfEnts[i].returning == true)
            {
                vectorOfEnts[i].xVelocity = 0.8f;
            }
            if(vectorOfEnts[i].startingPosition - vectorOfEnts[i].entityVector.x >= 4.8f)
            {
                vectorOfEnts[i].returning = true;
                vectorOfEnts[i].falling = false;
            }
            else if(vectorOfEnts[i].startingPosition <= vectorOfEnts[i].entityVector.x)
            {
                vectorOfEnts[i].falling = true;
                vectorOfEnts[i].returning = false;
            }
        }
        else if(vectorOfEnts[i].entityType == LEVEL_GOAL)
        {
            if(vectorOfEnts[0].collidesWith(&vectorOfEnts[i]))
            {
                endOfLevel = Mix_LoadWAV("endOfLevel.wav");
                Mix_PlayChannel(-1, endOfLevel, 0);
                progressToNextLevel(elapsed);
            }
        }
        else if(vectorOfEnts[i].entityType == DEATH_LOLLI)
        {
            if(vectorOfEnts[0].collidesWith(&vectorOfEnts[i]))
            {
                player1Alive = false;
            }
        }
        else if(vectorOfEnts[i].entityType == TURRET)
        {
            vectorOfEnts[i].timeFallen += elapsed;
            //cout << vectorOfEnts[i].timeFallen << endl;
            if(vectorOfEnts[i].timeFallen > 1.8f)
            {
                shootBullet(vectorOfEnts[i].entityVector.x, vectorOfEnts[i].entityVector.y, vectorOfEnts[i].rotation, bullets, false);
                vectorOfEnts[i].timeFallen = 0.0f;
            }
        }
    }
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), shouldRemoveBullet), bullets.end());
    for(int i = 0; i < bullets.size(); i++)
    {
        bullets[i].Update(elapsed);
    }
    for(int i = 0; i < bullets.size(); i++)
    {
        if(DetectCollisionBullet(vectorOfEnts[0], bullets[i]))
        {
            //cout << "Does this happen?" << endl;
            player1Alive = false;
            bullets.clear();
        }
    }
    vectorOfEnts[0].acceleration_x = 0.0f;
    if(!player1Alive)
    {
        vectorOfEnts[0].entityVector.x = 0.0f;
        vectorOfEnts[0].entityVector.y = 0.0f;
        timeDead+=elapsed;
        if(timeDead > 2.0f)
        {
            player1Alive = true;
            resetGame(currentLevel, vectorOfEnts);
            timeDead = 0.0f;
        }
    }
    if(vectorOfEnts[0].xVelocity > 4.0f)
    {
        vectorOfEnts[0].xVelocity = 4.0f;
    }
    else if(vectorOfEnts[0].xVelocity < -4.0f)
    {
        vectorOfEnts[0].xVelocity = -4.0f;
    }
}
void tileCollisionBot(float elapsed, Matrix &modelMatrix, ShaderProgram program, int index)
{
    int xTilePos = -1;
    int yTilePosBot = -1;
    float yValueToUse = 0.0f;
    if(vectorOfEnts[index].entityType == ENTITY_PLAYER || vectorOfEnts[index].entityType == ENTITY_FIST)
    {
        yValueToUse = vectorOfEnts[index].entityVector.y-(vectorOfEnts[index].height/2);
    }
    else if(vectorOfEnts[index].entityType == ENTITY_ICICLE)
    {
        yValueToUse = vectorOfEnts[index].entityVector.y;
    }

    worldToTileCoordinates((vectorOfEnts[index].entityVector.x), yValueToUse,
                            &xTilePos, &yTilePosBot);
    
    for(int j = 0; j < NUM_OF_SOLID_TILES; j++)
    {
        if(currentLevel == 1)
        {
            if(levelData[yTilePosBot][xTilePos] == solidTiles[j])
            {
                vectorOfEnts[index].collidedBot = true;
                if(vectorOfEnts[index].entityType == ENTITY_FIST)
                {
                    vectorOfEnts[index].fallen = true;
                    //cout << "I AM FALLEN :(!" << endl;
                }
            }
        }
        if(currentLevel == 2)
        {
            if(level2Data[yTilePosBot][xTilePos] == solidTiles[j])
            {
                vectorOfEnts[index].collidedBot = true;
                if(vectorOfEnts[index].entityType == ENTITY_FIST)
                {
                    vectorOfEnts[index].fallen = true;
                    //cout << "I AM FALLEN :(!" << endl;
                }
            }
        }
        if(currentLevel == 3)
        {
            if(level3Data[yTilePosBot][xTilePos] == solidTiles[j])
            {
                vectorOfEnts[index].collidedBot = true;
                if(vectorOfEnts[index].entityType == ENTITY_FIST)
                {
                    vectorOfEnts[index].fallen = true;
                    //cout << "I AM FALLEN :(!" << endl;
                }
            }
        }
    }
    if(vectorOfEnts[index].collidedBot)
    {
        vectorOfEnts[index].entityVector.y -= (yValueToUse - (-TILE_SIZE * yTilePosBot))+0.0001f;
        vectorOfEnts[index].yVelocity = 0.0f;
    }
}
void tileCollisionTop(float elapsed, Matrix &modelMatrix, ShaderProgram program, int index)
{
    int xTilePos = -1;
    int yTilePosTop = -1;
    worldToTileCoordinates((vectorOfEnts[index].entityVector.x),
                           (vectorOfEnts[index].entityVector.y)+(vectorOfEnts[index].height/2),
                           &xTilePos, &yTilePosTop);
    for(int j = 0; j < NUM_OF_SOLID_TILES; j++)
    {
        if(currentLevel == 1)
        {
            if(levelData[yTilePosTop][xTilePos] == solidTiles[j])
            {
                //cout << "pls no\n";
                vectorOfEnts[index].collidedTop = true;
            }
        }
        if(currentLevel == 2)
        {
            if(level2Data[yTilePosTop][xTilePos] == solidTiles[j])
            {
                //cout << "pls no\n";
                vectorOfEnts[index].collidedTop = true;
            }
        }
        if(currentLevel == 3)
        {
            if(level3Data[yTilePosTop][xTilePos] == solidTiles[j])
            {
                //cout << "pls no\n";
                vectorOfEnts[index].collidedTop = true;
            }
        }
    }
    if(vectorOfEnts[index].collidedTop)
    {
        if(vectorOfEnts[index].entityType == ENTITY_PLAYER)
        {
            vectorOfEnts[index].entityVector.y -= vectorOfEnts[index].entityVector.y + (vectorOfEnts[index].height/2)
         - ((-TILE_SIZE * yTilePosTop) - TILE_SIZE +0.0000001f);
            vectorOfEnts[index].yVelocity = 0.0f;
        }
    }
}
void tileCollisionLeft(float elapsed, Matrix &modelMatrix, ShaderProgram program, int index)
{
    int xTilePosLeft = -1;
    int yTilePos = -1;
    worldToTileCoordinates((vectorOfEnts[index].entityVector.x)-(vectorOfEnts[index].width/2),
                           vectorOfEnts[index].entityVector.y, &xTilePosLeft, &yTilePos);
    //cout << "x tile position: " << xTilePos << " y tile position: " << yTilePosBot << endl;
    for(int j = 0; j < NUM_OF_SOLID_TILES; j++)
    {
        if(vectorOfEnts[index].entityType == ENTITY_PLAYER)
        {
            if(currentLevel == 1)
            {
                if(levelData[yTilePos][xTilePosLeft] == solidTiles[j])
                {
                    //cout << "does this happen?" << endl;
                    vectorOfEnts[index].collidedLeft = true;
                }
            }
            if(currentLevel == 2)
            {
                if(level2Data[yTilePos][xTilePosLeft] == solidTiles[j])
                {
                    //cout << "does this happen?" << endl;
                    vectorOfEnts[index].collidedLeft = true;
                }
            }
            if(currentLevel == 3)
            {
                if(level3Data[yTilePos][xTilePosLeft] == solidTiles[j])
                {
                    //cout << "does this happen?" << endl;
                    vectorOfEnts[index].collidedLeft = true;
                }
            }
        }
    }
    if(vectorOfEnts[index].collidedLeft)
    {
        
        vectorOfEnts[index].entityVector.x -= (vectorOfEnts[index].entityVector.x -
                                          vectorOfEnts[index].width/2) - (TILE_SIZE * xTilePosLeft+TILE_SIZE)+0.0000001f;
        vectorOfEnts[index].xVelocity = 0.0f;
    }
}
void tileCollisionRight(float elapsed, Matrix &modelMatrix, ShaderProgram program, int index)
{
    int xTilePosRight = -1;
    int yTilePos = -1;
    worldToTileCoordinates((vectorOfEnts[index].entityVector.x)+(vectorOfEnts[index].width/2),
                           vectorOfEnts[index].entityVector.y, &xTilePosRight, &yTilePos);
    //cout << "x tile position: " << xTilePosRight << " y tile position: " << yTilePos << endl;
    for(int j = 0; j < NUM_OF_SOLID_TILES; j++)
    {
        if(vectorOfEnts[index].entityType == ENTITY_PLAYER)
        {
            if(currentLevel == 1)
            {
                if(levelData[yTilePos][xTilePosRight] == solidTiles[j])
                {
                    //cout << "Yo wassup witchu?" <<endl;
                    vectorOfEnts[index].collidedRight = true;
                }
            }
            if(currentLevel == 2)
            {
                if(level2Data[yTilePos][xTilePosRight] == solidTiles[j])
                {
                    //cout << "Yo wassup witchu?" <<endl;
                    vectorOfEnts[index].collidedRight = true;
                }
            }
            if(currentLevel == 3)
            {
                if(level3Data[yTilePos][xTilePosRight] == solidTiles[j])
                {
                    //cout << "Yo wassup witchu?" <<endl;
                    vectorOfEnts[index].collidedRight = true;
                }
            }
        }
    }
    if(vectorOfEnts[index].collidedRight)
    {
        //cout << "position of tilePosRight: " << TILE_SIZE * xTilePosRight << endl;
        vectorOfEnts[index].entityVector.x -= (vectorOfEnts[index].entityVector.x + (vectorOfEnts[index].width/2))-(TILE_SIZE * xTilePosRight)+0.000001f;
        vectorOfEnts[index].xVelocity = 0.0f;
    }
}

void setViewMatrix(ShaderProgram program, Matrix viewMatrix)
{
    viewMatrix.identity();
    /*
    float maxLowest = -3.0f;
    float maxLeftMost = 3.6f;
    float maxRightMost = 0.0f;
    float maxHighest = 0.0f;
    
    if(vectorOfEnts[0].entityVector.y > maxLowest && vectorOfEnts[0].entityVector.x > maxLeftMost)
    {
        viewMatrix.Translate(-vectorOfEnts[0].entityVector.x, -vectorOfEnts[0].entityVector.y, 0.0f);
    }
    else if(vectorOfEnts[0].entityVector.y < maxLowest && vectorOfEnts[0].entityVector.x > maxLeftMost)
    {
        viewMatrix.Translate(-vectorOfEnts[0].entityVector.x, -maxLowest, 0.0f);
    }
    else if(vectorOfEnts[0].entityVector.y > maxLowest && -vectorOfEnts[0].entityVector.x < 3.5f)
    {
        viewMatrix.Translate(-maxLeftMost, -vectorOfEnts[0].entityVector.y, 0.0f);
    }
    else if(vectorOfEnts[0].entityVector.y < maxLowest && vectorOfEnts[0].entityVector.x < maxLeftMost)
    {
        viewMatrix.Translate(-maxLeftMost, -maxLowest, 0.0f);
    }
     */
    viewMatrix.Translate(-3.64f, 3.26f, 0.0f);
    program.setViewMatrix(viewMatrix);

}
void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY)
{
    *gridX = (int)(worldX / TILE_SIZE);
    *gridY = (int)(-worldY / TILE_SIZE);
}
void RenderGameLevel(ShaderProgram program, Matrix modelMatrix)
{
    modelMatrix.identity();
    program.setModelMatrix(modelMatrix);
    renderTileLevel(&program);
    for(int i = 0; i < vectorOfEnts.size(); i++)
    {
        modelMatrix.identity();
        modelMatrix.Translate(vectorOfEnts[i].entityVector.x, vectorOfEnts[i].entityVector.y, 0.0f);
        modelMatrix.Scale(vectorOfEnts[i].rotation, 1.0f, 0.0f);
        program.setModelMatrix(modelMatrix);
        vectorOfEnts[i].Draw(program);
    }
    for(int i = 0; i < bullets.size(); i++)
    {
        bullets[i].Render(program, modelMatrix);
    }
    
}
void Render(ShaderProgram program, Matrix modelMatrix)
{
    switch(state)
    {
        case STATE_GAME_LEVEL1:
            RenderGameLevel(program, modelMatrix);
            break;
        case STATE_MAIN_MENU:
            RenderMainMenu(program, modelMatrix);
            break;
        case INGAME_MENU:
            renderInGameMenu(program, modelMatrix);
    }
}
void RenderMainMenu(ShaderProgram program, Matrix modelMatrix)
{
    //Background of game: (purple with stars)
    GLuint fontSheet = LoadTexture("font1.png", false);
    
    std::string text = "Try not to die HARDCORE!!";
    modelMatrix.identity();
    modelMatrix.Translate(-0.33f*8.5f, 1.7f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, text, 0.35f, -0.1f);
    
    std::string mainMenu = "Main Menu";
    modelMatrix.identity();
    modelMatrix.Translate(-0.3f*4.0f, 1.0f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, mainMenu, 0.38f, -0.1f);
    
    std::string playerOne = "Press 1 to play!";
    modelMatrix.identity();
    modelMatrix.Translate(-0.3f*5.6f, -0.05f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, playerOne, 0.25f, -0.1f);
    /*
    std::string playerTwo = "Press 2 for two player";
    modelMatrix.identity();
    modelMatrix.Translate(-0.3f*5.55f, -0.8f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, playerTwo, 0.25f, -0.1f);
     */
    
}
void renderInGameMenu(ShaderProgram program, Matrix modelMatrix)
{
    //cout << "AM I HERE?!" << endl;
    GLuint fontSheet = LoadTexture("font1.png", false);
    std::string text = "You're doing great!!";
    modelMatrix.identity();
    modelMatrix.Translate(-0.33f*8.5f, 1.7f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, text, 0.35f, -0.1f);
    
    std::string levelDisplay = "You're on level: ";
    modelMatrix.identity();
    modelMatrix.Translate(-0.33f*8.5f, 1.0f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, levelDisplay, 0.35f, -0.1f);
    
    std::ostringstream currentLevelConv;
    currentLevelConv << currentLevel;
    modelMatrix.identity();
    modelMatrix.Translate(1.8f, 1.0f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, currentLevelConv.str(), 0.35f, -0.1f);
    
    std::string quitOption = "If you would like to quit:";
    modelMatrix.identity();
    modelMatrix.Translate(-0.33f*8.8f, -0.5f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, quitOption, 0.35f, -0.1f);
    
    std::string quitOption1 = "Press x!";
    modelMatrix.identity();
    modelMatrix.Translate(-0.33f*4.4f, -0.8f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, quitOption1, 0.35f, -0.1f);
   
}
void setBackgroundColorAndClear()
{
    glClearColor(93.0f, 128.0f, 163.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
}
bool readHeader(ifstream &stream, int level)
{
    string line;
    while(getline(stream, line))
    {
        if(line == "") { break; }
        istringstream sStream(line);
        string key,value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if(key == "width")
        {
            mapWidth = atoi(value.c_str());
            //cout << "Map width: " << mapWidth << endl;
        } else if(key == "height")
        {
            mapHeight = atoi(value.c_str());
            //cout << "Map height: " << mapHeight << endl;
        }
    }
    if(mapWidth == -1 || mapHeight == -1)
    {
        return false;
    }
    else
    {
        // allocate our map data
        if(level == 1)
        {
            levelData = new unsigned int*[mapHeight];
            for(int i = 0; i < mapHeight; ++i)
            {
                levelData[i] = new unsigned int[mapWidth];
            }
        }
        if(level == 2)
        {
            level2Data = new unsigned int*[mapHeight];
            for(int i = 0; i < mapHeight; ++i)
            {
                level2Data[i] = new unsigned int[mapWidth];
            }
        }
        if(level == 3)
        {
            level3Data = new unsigned int*[mapHeight];
            for(int i = 0; i < mapHeight; ++i)
            {
                level3Data[i] = new unsigned int[mapWidth];
            }
        }
        return true;
    }
}
bool readLayerData(ifstream &stream, int level) {
    string line;
    while(getline(stream, line)) {
        if(line == "") { break; }
        istringstream sStream(line);
        string key,value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if(key == "data") {
            for(int y=0; y < mapHeight; y++) {
                getline(stream, line);
                //cout << line << endl;
                istringstream lineStream(line);
                string tile;
                for(int x=0; x < mapWidth; x++) {
                    getline(lineStream, tile, ',');
                    int val =  atoi(tile.c_str());
                    if(val > 0)
                    {
                        // be careful, the tiles in this format are indexed from 1 not 0
                        if(level == 1)
                        {
                            levelData[y][x] = val-1;
                        }
                        if(level == 2)
                        {
                            level2Data[y][x] = val-1;
                        }
                        if(level == 3)
                        {
                            level3Data[y][x] = val-1;
                        }
                        //cout << levelData[y][x] << endl;
                    }
                    else
                    {
                        //levelData[y][x] = 868;
                        if(level == 1)
                        {
                            levelData[y][x] = 868;
                        }
                        if(level == 2)
                        {
                            level2Data[y][x] = 868;
                        }
                        if(level == 3)
                        {
                            level3Data[y][x] = 868;
                        }
                        //cout << levelData[y][x] << endl;
                    }
                }
            }
        }
    }
    return true;
}

bool readEntityData(ifstream &stream)
{
    string line;
    string type;
    while(getline(stream, line))
    {
        if(line == "") { break; }
        istringstream sStream(line);
        string key,value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if(key == "type")
        {
            type = value;
        }
        else if(key == "location")
        {
            istringstream lineStream(value);
            string xPosition, yPosition;
            getline(lineStream, xPosition, ',');
            getline(lineStream, yPosition, ',');
           // float placeX = atoi(xPosition.c_str())/23*TILE_SIZE;
            //float placeY = atoi(yPosition.c_str())/23*-TILE_SIZE;
            //placeEntity(type, placeX, placeY);
        }
    }
    return true;
}
void readData(string file, int level)
{
    ifstream infile(file);
    string line;
    while (getline(infile, line)) {
        if(line == "[header]") {
            if(!readHeader(infile, level))
            {
                return;
            }
        } else if(line == "[layer]") {
            readLayerData(infile, level);
        } else if(line == "[ObjectLayer]") {
            //readEntityData(infile);
        }
    }
}

void renderTileLevel(ShaderProgram *program)
{
    float border = 3.0f/692.0f;
    vector<float> vertexData;
    vector<float> texCoordData;
    float u = 0.0f;
    float v = 0.0f;
    for(int y=0; y < mapHeight; y++)
    {
        for(int x=0; x < mapWidth; x++)
        {
            if(currentLevel == 1)
            {
                u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
                v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
            }
            if(currentLevel == 2)
            {
                u = (float)(((int)level2Data[y][x]) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
                v = (float)(((int)level2Data[y][x]) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
            }
            if(currentLevel == 3)
            {
                u = (float)(((int)level3Data[y][x]) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
                v = (float)(((int)level3Data[y][x]) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
            }
            float spriteWidth = (1.0f/(float)SPRITE_COUNT_X)-border*2;
            float spriteHeight = (1.0f/(float)SPRITE_COUNT_Y)-border*2;
            vertexData.insert(vertexData.end(),
                              {
                                  TILE_SIZE * x, -TILE_SIZE * y,
                                  TILE_SIZE * x, (-TILE_SIZE * y)-TILE_SIZE,
                                  (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                                  TILE_SIZE * x, -TILE_SIZE * y,
                                  (TILE_SIZE * x)+TILE_SIZE, (-TILE_SIZE * y)-TILE_SIZE,
                                  (TILE_SIZE * x)+TILE_SIZE, -TILE_SIZE * y
                              });
            texCoordData.insert(texCoordData.end(),
                                {
                                    u+border, v+border,
                                    u+border, v+(spriteHeight)+border,
                                    u+spriteWidth+border, v+(spriteHeight)+border,
                                    u+border, v+border,
                                    u+spriteWidth+border, v+(spriteHeight)+border,
                                    u+spriteWidth+border, v+border
                                });
        }
    }
    GLuint fontTexture = LoadTexture("spritesheet.png", false);
    glUseProgram(program->programID);
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glDrawArrays(GL_TRIANGLES, 0, (mapHeight * mapWidth * 6));
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
}

bool DetectCollisionBullet(Entity entityOne/* paddle */, Bullet bullet /*pongBall*/)
{
    //float yDifferenceBetweenTwoEntities = 0.0f;
    
    //x position of the left hand side of entityOne
    float entOneLeft = entityOne.entityVector.x-entityOne.width/2;
    //x position of the right hand side of entityOne
    float entOneRight = entityOne.entityVector.x+entityOne.width/2;
    //y position of the top of entityOne
    float entOneTop = entityOne.entityVector.y+entityOne.height/2;
    //y position of the bottom of entityOne
    float entOneBot = entityOne.entityVector.y-entityOne.height/2;
    //x position of the left hand side of bullet
    float entTwoLeft = bullet.x-bullet.width;
    //x position of the right hand side of bullet
    float entTwoRight = bullet.x+bullet.width;
    //y position of the top of bullet
    float entTwoTop = bullet.y+bullet.height;
    //y position of the bot of bullet
    float entTwoBot = bullet.y-bullet.height;
    
    //Box to box collision detection:
    /*
     a) is R1’s bottom higher than R2’s top?
     b) is R1’s top lower than R2’s bottom?
     c) is R1’s left larger than R2’s right?
     d) is R1’s right smaller than R2’s left
     */
    //The rectangles are intersecting if NONE of the above are true.
    
    if(!(entOneBot > entTwoTop) && !(entOneTop < entTwoBot) && !(entOneLeft > entTwoRight)
       && !(entOneRight < entTwoLeft))
    {
        return true;
    }
    else
    {
        return false;
    }
    
}



