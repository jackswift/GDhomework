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


#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

float MaxYPos = 2.0f;
float MaxXPos = 3.55f;
bool done = false;

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
void UpdateMainMenu(float elapsed, Matrix &modelMatrix, ShaderProgram program);
void UpdateGameLevel(float elapsed, Matrix &modelMatrix, ShaderProgram program);

//RENDER FUNCTIONS
void Render(ShaderProgram program, Matrix modelMatrix);
void RenderGameLevel(ShaderProgram program, Matrix modelMatrix);
void RenderMainMenu(ShaderProgram program, Matrix modelMatrix);
void RenderPlayerHit(ShaderProgram program, Matrix modelMatrix);

//INITIALIZATION OF ENTITY VECTOR
void initializeEntities();

//SOUND FUNCTIONS
float getAudioForTime(long numSamples);
void myAudioCallBack(void* userData, Uint8 *stream, int len);
float mixSamples(float A, float B);
int loadSound(const char* soundToLoad);
void playSound(int soundIndex, bool loop);

//COLLISION DETECTION (MIGHT GO INTO ENTITY FILE)
bool DetectCollision(Entity entityOne, Entity entityTwo);
bool DetectCollisionBullet(Entity entityOne/* paddle */, Bullet bullet /*bullet*/);


//Vector of entities
std::vector<Entity> vectorOfEnts;

//Vector of sounds
std::vector<MixerSound> mixSounds;
SDL_AudioSpec deviceSpec;


//Num samples for sounds
unsigned int numSamples = 0;

int main(int argc, char *argv[])
{
    
    state = STATE_MAIN_MENU;
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    Setup(projectionMatrix);
    float ticks;
    float lastFrameTicks = 0.0f;
    float elapsed;
    
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
    
    int jamesBrown = loadSound("feelgood.wav");
    
    playSound(jamesBrown, true);
    
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    //projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
    
    //Setup(program, projectionMatrix);
    
    //Animation, Matrix, and Entitys variables
    
    initializeEntities();
    
    //enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
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
        
        //std::cout << ticks << std::endl;
        
        //Update(elapsed, modelMatrix, program);
        
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
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            done = true;
        }
        
    }

}
void UpdateMainMenu(float elapsed, Matrix &modelMatrix, ShaderProgram program)
{
    //resetGame();
    //std::cout << vectorOfEnts.size() << std::endl;
}
void UpdateGameLevel(float elapsed, Matrix &modelMatrix, ShaderProgram program)
{
    
}
void Update(float elapsed, Matrix &modelMatrix, ShaderProgram program)
{
    switch(state)
    {
        case STATE_GAME_LEVEL:
            UpdateGameLevel(elapsed, modelMatrix, program);
            break;
        case STATE_MAIN_MENU:
            UpdateMainMenu(elapsed, modelMatrix, program);
            break;
    }
}


void RenderGameLevel(ShaderProgram program, Matrix modelMatrix)
{
    
}
void RenderMainMenu(ShaderProgram program, Matrix modelMatrix)
{
    
}
void RenderPlayerHit(ShaderProgram program, Matrix modelMatrix)
{
    
}
void Render(ShaderProgram program, Matrix modelMatrix)
{
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
}
void setBackgroundColorAndClear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
}
void initializeEntities()
{
    
}


bool DetectCollision(Entity entityOne/* paddle */, Entity entityTwo /*pongBall*/)
{
    //float yDifferenceBetweenTwoEntities = 0.0f;
    
    //x position of the left hand side of entityOne
    float entOneLeft = entityOne.xPosition-entityOne.width;
    //x position of the right hand side of entityOne
    float entOneRight = entityOne.xPosition+entityOne.width;
    //y position of the top of entityOne
    float entOneTop = entityOne.yPosition+entityOne.height;
    //y position of the bottom of entityOne
    float entOneBot = entityOne.yPosition-entityOne.height;
    //x position of the left hand side of entityTwo
    float entTwoLeft = entityTwo.xPosition-entityTwo.width;
    //x position of the right hand side of entityTwo
    float entTwoRight = entityTwo.xPosition+entityTwo.width;
    //y position of the top of entityTwo
    float entTwoTop = entityTwo.yPosition+entityTwo.height;
    //y position of the bot of entityTwo
    float entTwoBot = entityTwo.yPosition-entityTwo.height;
    
    
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
bool DetectCollisionBullet(Entity entityOne/* paddle */, Bullet bullet /*pongBall*/)
{
    //float yDifferenceBetweenTwoEntities = 0.0f;
    
    //x position of the left hand side of entityOne
    float entOneLeft = entityOne.xPosition-entityOne.sprite.width;
    //x position of the right hand side of entityOne
    float entOneRight = entityOne.xPosition+entityOne.sprite.width;
    //y position of the top of entityOne
    float entOneTop = entityOne.yPosition+entityOne.sprite.height;
    //y position of the bottom of entityOne
    float entOneBot = entityOne.yPosition-entityOne.sprite.height;
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



