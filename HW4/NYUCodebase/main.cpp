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

float MaxYPos = 2.0f;
float MaxXPos = 3.55f;
bool done = false;
int mapWidth = -1;
int mapHeight = -1;
int solidTiles[5] = {121, 154, -1, -1, -1};



const int numFrames = 5;
float animationElapsed = 0.0f;
float framesPerSecond = 30.0f;
int currentIndex = 1;


//Gamestate enum and state
enum GameState {STATE_MAIN_MENU, STATE_GAME_LEVEL, PLAYER_HIT};
int state;

const Uint8 *keys = SDL_GetKeyboardState(NULL);

SDL_Window* displayWindow;


//SETUP AND EVENTS FUNCTIONS
void setBackgroundColorAndClear();
void Setup(Matrix &projectionMatrix);
void ProcessEvents();

//UPDATE FUNCTIONS
void Update(float elapsed, Matrix &modelMatrix, ShaderProgram program);
void tileCollisionBot(float elapsed, Matrix &modelMatrix, ShaderProgram program, int index);
void tileCollisionTop(float elapsed, Matrix &modelMatrix, ShaderProgram program, int index);
void tileCollisionLeft(float elapsed, Matrix &modelMatrix, ShaderProgram program, int index);
void tileCollisionRight(float elapsed, Matrix &modelMatrix, ShaderProgram program, int index);

//RENDER FUNCTIONS
void Render(ShaderProgram program, Matrix modelMatrix);
void renderTileLevel(ShaderProgram *program);
void setViewMatrix(ShaderProgram program, Matrix viewMatrix);

//INITIALIZATION OF ENTITY VECTOR
void initializeEntities();
void placeEntity(string type, float xPosition, float yPosition);

//SOUND FUNCTIONS
float getAudioForTime(long numSamples);
void myAudioCallBack(void* userData, Uint8 *stream, int len);
float mixSamples(float A, float B);
int loadSound(const char* soundToLoad);
void playSound(int soundIndex, bool loop);

//Tile stuff
void readData();
bool readEntityData(ifstream &stream);
bool readLayerData(ifstream &stream);
bool readHeader(ifstream &stream);

void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY);

//COLLISION DETECTION (MIGHT GO INTO ENTITY FILE)
//bool DetectCollision(Entity entityOne, Entity entityTwo);
//bool DetectCollisionBullet(Entity entityOne/* paddle */, Bullet bullet /*bullet*/);


//Vector of entities
std::vector<Entity> vectorOfEnts;

//Vector of sounds
std::vector<MixerSound> mixSounds;
SDL_AudioSpec deviceSpec;


//Num samples for sounds
unsigned int numSamples = 0;

int main(int argc, char *argv[])
{
    
    //state = STATE_MAIN_MENU;
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    Setup(projectionMatrix);
    float ticks;
    float lastFrameTicks = 0.0f;
    float elapsed;
    
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    SDL_AudioSpec requestSpec;
    requestSpec.freq = 44100;
    requestSpec.format = AUDIO_F32;
    requestSpec.samples = 512;
    requestSpec.channels = 1;
    requestSpec.userdata = NULL;
    requestSpec.callback = myAudioCallBack;
    
    //deviceSpec.userdata = (void*)this;
    SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &requestSpec, &deviceSpec, SDL_AUDIO_ALLOW_ANY_CHANGE);
    
    SDL_PauseAudioDevice(dev, 0);
    
    initializeEntities();
    
    //enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    readData();
    for(int i = 0; i < 24; i++)
    {
        for(int j = 0; j < 48; j++)
        {
            //cout << levelData[i][j] << endl;
        }
    }
    
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
        
        
        
        float fixedElapsed = elapsed;
        if(fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
            fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
        }
        while (fixedElapsed >= FIXED_TIMESTEP ) {
            fixedElapsed -= FIXED_TIMESTEP;
            Update(FIXED_TIMESTEP, modelMatrix, program);
        }
        Update(fixedElapsed, modelMatrix, program);
        
        setViewMatrix(program, viewMatrix);
        
        Render(program, modelMatrix);
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
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
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            done = true;
        }
        if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.scancode == SDL_SCANCODE_SPACE)
            {
                //only allows jumping when the entity is touching the ground
                if(vectorOfEnts[0].collidedBot)
                {
                    vectorOfEnts[0].yVelocity = 1.2f;
                }
                //else do nothing
            }
        }
        
    }
}
void Update(float elapsed, Matrix &modelMatrix, ShaderProgram program)
{
    int animationIndex[5] = {19, 26, 27, 28, 29};
    if(keys[SDL_SCANCODE_D])
    {
        animationElapsed += elapsed;
        vectorOfEnts[0].acceleration_x = 1.0f;
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
        if(vectorOfEnts[0].xPosition > 0.0f)
        {
            vectorOfEnts[0].acceleration_x = -1.0f;
        }
        else
        {
            vectorOfEnts[0].xVelocity = 0.0f;
        }
    }
    if(vectorOfEnts[0].acceleration_x <= 0.0f)
    {
        vectorOfEnts[0].setSprite(SPRITE_COUNT_Y, SPRITE_COUNT_X, animationIndex[0], 0.17f);
    }
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
            vectorOfEnts[i].UpdateX(elapsed);
            //tileCollisionLeft(elapsed, modelMatrix, program, i);
            tileCollisionRight(elapsed, modelMatrix, program, i);
        }
    }
    vectorOfEnts[0].acceleration_x = 0.0f;
}
void tileCollisionBot(float elapsed, Matrix &modelMatrix, ShaderProgram program, int index)
{
    int xTilePos = -1;
    int yTilePosBot = -1;
    worldToTileCoordinates((vectorOfEnts[index].xPosition),
                           vectorOfEnts[index].yPosition-(vectorOfEnts[index].height/2),
                           &xTilePos, &yTilePosBot);
    for(int j = 0; j < 5; j++)
    {
        if(levelData[yTilePosBot][xTilePos] == solidTiles[j])
        {
            vectorOfEnts[index].collidedBot = true;
        }
    }
    if(vectorOfEnts[index].collidedBot)
    {
        vectorOfEnts[index].yPosition -= (vectorOfEnts[index].yPosition -
                                          (vectorOfEnts[index].height/2) - (-TILE_SIZE * yTilePosBot))+0.0001f;
        vectorOfEnts[index].yVelocity = 0.0f;
    }
}
void tileCollisionTop(float elapsed, Matrix &modelMatrix, ShaderProgram program, int index)
{
    int xTilePos = -1;
    int yTilePosTop = -1;
    worldToTileCoordinates((vectorOfEnts[index].xPosition),
                           (vectorOfEnts[index].yPosition)+(vectorOfEnts[index].height/2),
                           &xTilePos, &yTilePosTop);
    for(int j = 0; j < 5; j++)
    {
        if(levelData[yTilePosTop][xTilePos] == solidTiles[j])
        {
            vectorOfEnts[index].collidedTop = true;
        }
    }
    if(vectorOfEnts[index].collidedTop)
    {
        vectorOfEnts[index].yPosition -= vectorOfEnts[index].yPosition + (vectorOfEnts[index].height/2)
         - ((-TILE_SIZE * yTilePosTop) - TILE_SIZE +0.0000001f);
        vectorOfEnts[index].yVelocity = 0.0f;
    }
}
void tileCollisionLeft(float elapsed, Matrix &modelMatrix, ShaderProgram program, int index)
{
    int xTilePosLeft = -1;
    int yTilePos = -1;
    worldToTileCoordinates((vectorOfEnts[index].xPosition)-(vectorOfEnts[index].width/2),
                           vectorOfEnts[index].yPosition, &xTilePosLeft, &yTilePos);
    //cout << "x tile position: " << xTilePos << " y tile position: " << yTilePosBot << endl;
    for(int j = 0; j < 5; j++)
    {
        if(levelData[yTilePos][xTilePosLeft] == solidTiles[j])
        {
            cout << "does this happen?" << endl;
            vectorOfEnts[index].collidedLeft = true;
        }
    }
    if(vectorOfEnts[index].collidedLeft)
    {
        vectorOfEnts[index].xPosition -= (vectorOfEnts[index].xPosition +
                                          (vectorOfEnts[index].width/2) - (TILE_SIZE * xTilePosLeft))+0.0000001f;
        vectorOfEnts[index].xVelocity = 0.0f;
    }
}
void tileCollisionRight(float elapsed, Matrix &modelMatrix, ShaderProgram program, int index)
{
    int xTilePosRight = -1;
    int yTilePos = -1;
    worldToTileCoordinates((vectorOfEnts[index].xPosition)+(vectorOfEnts[index].width/2),
                           vectorOfEnts[index].yPosition, &xTilePosRight, &yTilePos);
    //cout << "x tile position: " << xTilePosRight << " y tile position: " << yTilePos << endl;
    for(int j = 0; j < 5; j++)
    {
        if(levelData[yTilePos][xTilePosRight] == solidTiles[j])
        {
            vectorOfEnts[index].collidedRight = true;
        }
    }
    if(vectorOfEnts[index].collidedRight)
    {
        //cout << "position of tilePosRight: " << TILE_SIZE * xTilePosRight << endl;
        vectorOfEnts[index].xPosition -= (vectorOfEnts[index].xPosition + (vectorOfEnts[index].width/2))-(TILE_SIZE * xTilePosRight)+0.000001f;
        vectorOfEnts[index].xVelocity = 0.0f;
    }
}

void setViewMatrix(ShaderProgram program, Matrix viewMatrix)
{
    viewMatrix.identity();
    float maxLowest = -3.4f;
    float maxLeftMost = 3.5f;
    
    if(vectorOfEnts[0].yPosition > maxLowest && vectorOfEnts[0].xPosition > maxLeftMost)
    {
        viewMatrix.Translate(-vectorOfEnts[0].xPosition, -vectorOfEnts[0].yPosition, 0.0f);
    }
    else if(vectorOfEnts[0].yPosition < maxLowest && vectorOfEnts[0].xPosition > maxLeftMost)
    {
        viewMatrix.Translate(-vectorOfEnts[0].xPosition, -maxLowest, 0.0f);
    }
    else if(vectorOfEnts[0].yPosition > maxLowest && -vectorOfEnts[0].xPosition < 3.5f)
    {
        viewMatrix.Translate(-maxLeftMost, -vectorOfEnts[0].yPosition, 0.0f);
    }
    else if(vectorOfEnts[0].yPosition < maxLowest && vectorOfEnts[0].xPosition < maxLeftMost)
    {
        viewMatrix.Translate(-maxLeftMost, -maxLowest, 0.0f);
    }
    program.setViewMatrix(viewMatrix);

}
void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY)
{
    *gridX = (int)(worldX / TILE_SIZE);
    *gridY = (int)(-worldY / TILE_SIZE);
}
void Render(ShaderProgram program, Matrix modelMatrix)
{
    /*
    switch(state)
    {
        case STATE_GAME_LEVEL:
            RenderGameLevel(program, modelMatrix);
            break;
        case STATE_MAIN_MENU:
            RenderMainMenu(program, modelMatrix);
            break;
        case PLAYER_HIT:
            RenderPlayerHit(program, modelMatrix);
            break;
    }
     */
    /*
    for(int i = 0; i < vectorOfEnts.size(); i++)
    {
        modelMatrix.identity();
        modelMatrix.Translate(vectorOfEnts[i].xPosition, vectorOfEnts[i].yPosition, 0.0f);
        program.setModelMatrix(modelMatrix);
        vectorOfEnts[i].Draw(program);
    }
     */
    modelMatrix.identity();
    program.setModelMatrix(modelMatrix);
    renderTileLevel(&program);
    for(int i = 0; i < vectorOfEnts.size(); i++)
    {
        modelMatrix.identity();
        modelMatrix.Translate(vectorOfEnts[i].xPosition, (vectorOfEnts[i].yPosition), 0.0f);
        program.setModelMatrix(modelMatrix);
        vectorOfEnts[i].Draw(program);
    }
    
}
void setBackgroundColorAndClear()
{
    glClearColor(93.0f, 128.0f, 163.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
}

void initializeEntities()
{
    int index = 19;
    float u = (float)(((int)index) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
    float v = (float)(((int)index) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
    float spriteWidth = 1.0/(float)SPRITE_COUNT_X;
    float spriteHeight = 1.0/(float)SPRITE_COUNT_Y;
    GLuint playerTexture = LoadTexture("spritesheet.png", false);
    Entity playerEntity;
    playerEntity.entityType = ENTITY_PLAYER;
    playerEntity.EntityTexture = playerTexture;
    playerEntity.xVelocity = 0;
    playerEntity.usesSprite = true;
    playerEntity.sprite = SheetSprite(playerTexture, u, v, spriteWidth, spriteHeight, 0.17f);
    playerEntity.height = 0.17f;
    playerEntity.width = 0.17f;
    playerEntity.isStatic = false;
    playerEntity.gravity_y = -1.2f;
    playerEntity.acceleration_x = 0.0f;
    playerEntity.acceleration_y = 0.0f;
    playerEntity.xPosition = 7.5f;
    playerEntity.yPosition = -3.0f;
    playerEntity.xFriction = 1.5f;
    playerEntity.entityVector.x = playerEntity.xPosition = 7.5f;
    playerEntity.entityVector.y = -3.0f;
    playerEntity.entityVector.z = 0.0f;
    vectorOfEnts.push_back(playerEntity);
}

float getAudioForTime(long numSamples)
{
    double elapsed = ((double)numSamples)/44100.0;
    return sin(elapsed * 2.0 * M_PI * 350.0);
}
int loadSound(const char* soundToLoad)
{
    Uint8 *buffer;
    SDL_AudioSpec spec;
    Uint32 bufferSize;
    if(SDL_LoadWAV(soundToLoad, &spec, &buffer, &bufferSize) == NULL)
    {
        return -1;
    }
    SDL_AudioCVT cvt;
    if(SDL_BuildAudioCVT(&cvt, spec.format, spec.channels, spec.freq, deviceSpec.format, deviceSpec.channels, deviceSpec.freq) == -1) {
        return -1;
    }
    cvt.len = bufferSize;
    cvt.buf = new Uint8[bufferSize * cvt.len_mult];
    
    memcpy(cvt.buf, buffer, bufferSize);
    
    SDL_ConvertAudio(&cvt);
    SDL_FreeWAV(buffer);
    
    MixerSound sound;
    sound.buffer = cvt.buf;
    sound.length = cvt.len_cvt;
    sound.loaded = true;
    sound.offset = 0;
    sound.format = deviceSpec.format;
    sound.volume = 1.0;
    sound.playing = false;
    mixSounds.push_back(sound);
    return mixSounds.size()-1;
}
void myAudioCallBack(void* userData, Uint8 *stream, int len)
{
    memset(stream, 0, len);
    
    for(int i = 0; i < mixSounds.size(); i++)
    {
        MixerSound *sound = &mixSounds[i];
        if(sound->loaded && sound->playing)
        {
            for(int s = 0; s < len/4; s++)
            {
                float *sourceBuffer = (float*)(sound->buffer + sound->offset);
                ((float*)stream)[s] = mixSamples(((float*)stream)[s], (sourceBuffer[s]*sound->volume));
            }
            sound->offset += len;
            if(sound->offset >= sound->length - len)
            {
                if(sound->loop)
                {
                    sound->offset = 0;
                }
                else{
                    sound->playing = false;
                }
            }
        }
    }
}
float mixSamples(float A, float B) {
    if (A < 0 && B < 0 ) {
        return  (A + B) - (A * B)/-1.0;
    } else if (A > 0 && B > 0 ) {
        return (A + B) - (A * B)/1.0;
    } else {
        return A + B;
    }
}
void playSound(int soundIndex, bool loop)
{
    if(soundIndex >= 0 && soundIndex < mixSounds.size())
    {
        mixSounds[soundIndex].playing = true;
        mixSounds[soundIndex].offset = 0;
        mixSounds[soundIndex].loop = loop;
    }
}
bool readHeader(ifstream &stream)
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
        levelData = new unsigned int*[mapHeight];
        for(int i = 0; i < mapHeight; ++i)
        {
            levelData[i] = new unsigned int[mapWidth];
        }
        return true;
    }
}
bool readLayerData(ifstream &stream) {
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
                        levelData[y][x] = val-1;
                        //cout << levelData[y][x] << endl;
                    }
                    else
                    {
                        levelData[y][x] = 0;
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
            float placeX = atoi(xPosition.c_str())/23*TILE_SIZE;
            float placeY = atoi(yPosition.c_str())/23*-TILE_SIZE;
            placeEntity(type, placeX, placeY);
        }
    }
    return true;
}
void placeEntity(string type, float xPosition, float yPosition)
{
    
}
void readData()
{
    ifstream infile("/Users/lyletrue/MyGameDesignHW2/HW4/NYUCodebase/file.txt");
    string line;
    while (getline(infile, line)) {
        if(line == "[header]") {
            if(!readHeader(infile))
            {
                return;
            }
        } else if(line == "[layer]") {
            readLayerData(infile);
        } else if(line == "[ObjectsLayer]") {
            readEntityData(infile);
        }
    }
}

void renderTileLevel(ShaderProgram *program)
{
    float border = 3.0f/692.0f;
    vector<float> vertexData;
    vector<float> texCoordData;
    
    for(int y=0; y < mapHeight; y++)
    {
        for(int x=0; x < mapWidth; x++)
        {
            /*
            float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
            float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
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
             */
            
            if(levelData[y][x] != 0)
            {
                float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
                float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
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
    }
    GLuint fontTexture = LoadTexture("spritesheet.png", false);
    glUseProgram(program->programID);
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glDrawArrays(GL_TRIANGLES, 0, mapHeight * mapWidth * 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
}



