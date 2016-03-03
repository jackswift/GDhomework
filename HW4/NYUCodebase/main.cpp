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

std::vector<Bullet> playerBullets;
std::string playerScoreStr;
std::vector<Bullet> enemyBullets;
//float ticks;
//float lastFrameTicks = 0.0f;
//float elapsed;

enum GameState {STATE_MAIN_MENU, STATE_GAME_LEVEL, PLAYER_HIT};

int state;

int playerScore = 0;
int playerLives = 2;
const Uint8 *keys = SDL_GetKeyboardState(NULL);

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path, bool isRepeating);
void setBackgroundColorAndClear();
void drawTexture(GLuint theTexture, ShaderProgram program, float height = 0.5f, float width = 0.5f);
void Setup(Matrix &projectionMatrix);

void ProcessEvents();

void Update(float elapsed, Matrix &modelMatrix, ShaderProgram program);
void UpdateMainMenu(float elapsed, Matrix &modelMatrix, ShaderProgram program);
void UpdateGameLevel(float elapsed, Matrix &modelMatrix, ShaderProgram program);
void Render(ShaderProgram program, Matrix modelMatrix);
void RenderGameLevel(ShaderProgram program, Matrix modelMatrix);
void RenderMainMenu(ShaderProgram program, Matrix modelMatrix);
void DrawspriteSprite(ShaderProgram &program, GLuint theTexture, int index, int SpriteXCount, int SpriteYCount);
void resetGame(bool gameOver);
void userGetsHit(ShaderProgram program, Matrix modelMatrix);
void RenderPlayerHit(ShaderProgram program, Matrix modelMatrix);

void initializeEntities();

//Sheet Sprite for non-uniform sheets
class SheetSprite {
public:
    SheetSprite() {}
    SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size) : textureID(textureID), u(u), v(v), width(width), height(height), size(size) {}
    void Draw(ShaderProgram program);
    float size;
    unsigned int textureID;
    float u;
    float v;
    float width;
    float height;
};

class Entity
{
public:
    void Draw(ShaderProgram program);
    
    void Update(float elapsed);
    float xPosition;
    float yPosition;
    float rotation;
    
    float height;
    float width;
    
    float speed;
    float xVelocity;
    float yVelocity;

    
    GLuint EntityTexture;
    SheetSprite sprite;
    bool usesSprite;
    //Matrix EntityModelMatrix;
    //float xAcceleration;
    //float yAcceleration;
private:
    
};

int findNextLastEntity(std::vector<Entity> entities, int column, int posToNotCheck);

std::vector<Entity> vectorOfEnts;

void Entity::Update(float elapsed)
{
    xPosition += (xVelocity * speed) * elapsed;
    yPosition += (yVelocity * speed) * elapsed;
}
void Entity::Draw(ShaderProgram program)
{
    //program.setModelMatrix(EntityModelMatrix);
    if(!usesSprite)
    {
        drawTexture(EntityTexture, program, height, width);
    }
    else
    {
        sprite.Draw(program);
    }
}

void SheetSprite::Draw(ShaderProgram program) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLfloat texCoords[] = {
        u, v+height,
        u+width, v,
        u, v,
        u+width, v,
        u, v+height,
        u+width, v+height
    };
    float aspect = width / height;
    float vertices[] = {
        -0.5f * size * aspect, -0.5f * size,
        0.5f * size * aspect, 0.5f * size,
        -0.5f * size * aspect, 0.5f * size,
        0.5f * size * aspect, 0.5f * size,
        -0.5f * size * aspect, -0.5f * size ,
        0.5f * size * aspect, -0.5f * size};
    // draw our arrays
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}

bool DetectCollision(Entity entityOne, Entity entityTwo);
bool DetectCollisionBullet(Entity entityOne/* paddle */, Bullet bullet /*bullet*/);
//Entity p1Paddle;
//Entity pongBall;
//Entity p2Paddle;
//Entity p1Score;
//Entity p2Score;

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
    
    //int x = 200;
    
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
    SDL_Init(SDL_INIT_VIDEO);
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
void userGetsHit(ShaderProgram program, Matrix modelMatrix)
{
    
}
void resetGame(bool gameOver)
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



