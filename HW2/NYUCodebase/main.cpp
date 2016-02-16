#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif


#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

int p1ScoreCount = 0;
int p2ScoreCount = 0;
float MaxYPos = 2.0f;
float MaxXPos = 3.55f;
bool done = false;


float ticks;
float lastFrameTicks = 0.0f;
float elapsed;


const Uint8 *keys = SDL_GetKeyboardState(NULL);

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path);
void setBackgroundColorAndClear();
void drawTexture(GLuint theTexture, ShaderProgram program, float height = 0.5f, float width = 0.5f);
void Setup(ShaderProgram program, Matrix &projectionMatrix);

void ProcessEvents();

void Update(float lastFrameTicks, Matrix &modelMatrix, ShaderProgram program);
void Render(ShaderProgram program, Matrix modelMatrix);
void DrawSpriteSheetSprite(ShaderProgram &program, GLuint theTexture, int index, int SpriteXCount, int SpriteYCount);

void initializeEntities();

class Entity
{
public:
    void Draw(ShaderProgram program);
    
    float xPosition;
    float yPosition;
    float rotation;
    
    float height;
    float width;
    
    float speed;
    float xVelocity;
    float yVelocity;
    
    GLuint EntityTexture;
    //Matrix EntityModelMatrix;
    //float xAcceleration;
    //float yAcceleration;
private:
    
};

void Entity::Draw(ShaderProgram program)
{
    //program.setModelMatrix(EntityModelMatrix);
    drawTexture(EntityTexture, program, height, width);
    
}
int DetectCollision(Entity entityOne, Entity entityTwo);

Entity p1Paddle;
Entity pongBall;
Entity p2Paddle;
Entity p1Score;
Entity p2Score;

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 360);
    srand(time(0));
    
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
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
        
        Update(lastFrameTicks, modelMatrix, program);
        
        Render(program, modelMatrix);
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
/*
void Setup(ShaderProgram program, Matrix &projectionMatrix)
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
    //Program and window stuff
    
    glViewport(0, 0, 640, 360);
    
}
*/
void ProcessEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.scancode == SDL_SCANCODE_SPACE && pongBall.xVelocity == 0.0f && pongBall.yVelocity == 0.0f)
            {
                pongBall.xVelocity = (float(rand() % 100 + 50))/100;
                pongBall.yVelocity = (float(rand() % 100 + 50))/100;
            }
        }
    }

}

void Update(float lastFrameTicks, Matrix &modelMatrix, ShaderProgram program)
{
    int collisionTest = 0;
    
    if(keys[SDL_SCANCODE_W])
    {
        p1Paddle.yVelocity = 1.0f;
        p1Paddle.yPosition += elapsed * (p1Paddle.speed * p1Paddle.yVelocity);
        if(p1Paddle.yPosition > (MaxYPos-(p1Paddle.height)))
        {
            p1Paddle.speed = 0.0f;
        }
        else
        {
            p1Paddle.speed = 2.4f;
        }
    }
    else if(keys[SDL_SCANCODE_S])
    {
        p1Paddle.yVelocity = -1.0f;
        p1Paddle.yPosition += elapsed * (p1Paddle.speed * p1Paddle.yVelocity);
        if(p1Paddle.yPosition < ((-MaxYPos)+(p1Paddle.height)))
        {
            p1Paddle.speed = 0.0f;
        }
        else
        {
            p1Paddle.speed = 2.4f;
        }
    }
    
    pongBall.xPosition += elapsed * (pongBall.speed * pongBall.xVelocity);
    pongBall.yPosition += elapsed * (pongBall.speed * pongBall.yVelocity);
    if(pongBall.xPosition > MaxXPos-pongBall.width)
    {
        p1ScoreCount++;
        glClearColor(0.0f, 255.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        pongBall.xVelocity = 0.0f;
        pongBall.xPosition = 0.0f;
        pongBall.yPosition = 0.0f;
        pongBall.yVelocity = 0.0f;
    }
    if(pongBall.xPosition < (-MaxXPos)+pongBall.width)
    {
        p2ScoreCount++;
        glClearColor(255.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        pongBall.xVelocity = 0.0f;
        pongBall.xPosition = 0.0f;
        pongBall.yPosition = 0.0f;
        pongBall.yVelocity = 0.0f;
        
    }
    if(pongBall.yPosition > MaxYPos-pongBall.height || pongBall.yPosition < (-MaxYPos)+pongBall.height)
    {
        pongBall.yVelocity = -pongBall.yVelocity;
        if(pongBall.yPosition > 0)
        {
            pongBall.yPosition -= 0.1f;
        }
        else
        {
            pongBall.yPosition += 0.1f;
        }
    }
    //collisionTest = DetectCollision(p1Paddle, pongBall);
    //if collision tests positive, OR pongBall hits the right wall:
    // || pongBall.xPosition > MaxXPos-pongBall.width)
    if((collisionTest = DetectCollision(p1Paddle, pongBall)))
    {
        pongBall.speed += 0.1;
        std::cout << collisionTest << std::endl;
        //switch xVelocity of ball (happens no matter what)
        pongBall.xVelocity = -pongBall.xVelocity;
        //now the collision tests is testing for where on the paddle
        //the ball is hitting (top 3rd, middle 3rd, bottom 3rd) (changes y Trajectory
        //based off of where it hits)
        //(TOP THIRD)
        if(collisionTest == 3)
        {
            pongBall.yVelocity = (sqrtf(2.0)/2.0f);
            pongBall.xVelocity = (sqrtf(2.0)/2.0f);
            if(pongBall.yVelocity < 0.0f)
            {
                pongBall.yVelocity = -pongBall.yVelocity;
            }
        }
        //(BOT THIRD)
        else if(collisionTest == 2)
        {
            pongBall.yVelocity = (sqrtf(2.0)/2.0f);
            pongBall.xVelocity = (sqrtf(2.0)/2.0f);
            if(pongBall.yVelocity > 0.0f)
            {
                pongBall.yVelocity = -pongBall.yVelocity;
            }
        }
        //(MIDDLE THIRD)
        else
        {
            pongBall.xVelocity = 1.0f;
            pongBall.yVelocity = 0.0f;
        }
    }
    if ((collisionTest = DetectCollision(p2Paddle, pongBall)))
    {
        pongBall.speed += 0.1;
        std::cout << collisionTest << std::endl;
        //switch xVelocity of ball (happens no matter what)
        pongBall.xVelocity = -pongBall.xVelocity;
        //now the collision tests is testing for where on the paddle
        //the ball is hitting (top 3rd, middle 3rd, bottom 3rd) (changes y Trajectory
        //based off of where it hits)
        //(TOP THIRD)
        if(collisionTest == 3)
        {
            pongBall.yVelocity = -(sqrtf(2.0)/2.0f);
            pongBall.xVelocity = -(sqrtf(2.0)/2.0f);
            if(pongBall.yVelocity < 0.0f)
            {
                pongBall.yVelocity = -pongBall.yVelocity;
            }
        }
        //(BOT THIRD)
        else if(collisionTest == 2)
        {
            
            pongBall.yVelocity = -(sqrtf(2.0)/2.0f);
            pongBall.xVelocity = -(sqrtf(2.0)/2.0f);
            if(pongBall.yVelocity > 0.0f)
            {
                pongBall.yVelocity = -pongBall.yVelocity;
            }
        }
        //(MIDDLE THIRD)
        else
        {
            pongBall.xVelocity = -1.0f;
            pongBall.yVelocity = 0.0f;
        }
    }
    if(pongBall.xPosition > MaxXPos-pongBall.width)
    {
        pongBall.xVelocity = -pongBall.xVelocity;
    }
    
    if(keys[SDL_SCANCODE_UP])
    {
        p2Paddle.yVelocity = 1.0f;
        p2Paddle.yPosition += elapsed * (p2Paddle.speed * p2Paddle.yVelocity);
        if(p2Paddle.yPosition > (MaxYPos-(p2Paddle.height)))
        {
            p2Paddle.speed = 0.0f;
        }
        else
        {
            p2Paddle.speed = 2.4f;
        }
    }
    else if(keys[SDL_SCANCODE_DOWN])
    {
        p2Paddle.yVelocity = -1.0f;
        p2Paddle.yPosition += elapsed * (p2Paddle.speed * p2Paddle.yVelocity);
        if(p2Paddle.yPosition < ((-MaxYPos)+(p2Paddle.height)))
        {
            p2Paddle.speed = 0.0f;
        }
        else
        {
            p2Paddle.speed = 2.4f;
        }
    }
    //std::cout << p1ScoreCount << std::endl;
    
}

void Render(ShaderProgram program, Matrix modelMatrix)
{
    modelMatrix.identity();
    modelMatrix.Translate(p1Score.xPosition, p1Score.yPosition, 0.0f);
    program.setModelMatrix(modelMatrix);
    switch (p1ScoreCount) {
        case 0:
            DrawSpriteSheetSprite(program, p1Score.EntityTexture, 48, 16, 16);
            break;
        case 1:
            DrawSpriteSheetSprite(program, p1Score.EntityTexture, 49, 16, 16);
            break;
        case 2:
            DrawSpriteSheetSprite(program, p1Score.EntityTexture, 50, 16, 16);
            break;
        case 3:
            DrawSpriteSheetSprite(program, p1Score.EntityTexture, 51, 16, 16);
            break;
        case 4:
            DrawSpriteSheetSprite(program, p1Score.EntityTexture, 52, 16, 16);
            break;
        case 5:
            DrawSpriteSheetSprite(program, p1Score.EntityTexture, 53, 16, 16);
            break;
        case 6:
            DrawSpriteSheetSprite(program, p1Score.EntityTexture, 54, 16, 16);
            break;
        default:
            break;
    }
    
    modelMatrix.identity();
    modelMatrix.Translate(p2Score.xPosition, p2Score.yPosition, 0.0f);
    program.setModelMatrix(modelMatrix);
    switch (p2ScoreCount) {
        case 0:
            DrawSpriteSheetSprite(program, p2Score.EntityTexture, 48, 16, 16);
            break;
        case 1:
            DrawSpriteSheetSprite(program, p2Score.EntityTexture, 49, 16, 16);
            break;
        case 2:
            DrawSpriteSheetSprite(program, p2Score.EntityTexture, 50, 16, 16);
            break;
        case 3:
            DrawSpriteSheetSprite(program, p2Score.EntityTexture, 51, 16, 16);
            break;
        case 4:
            DrawSpriteSheetSprite(program, p2Score.EntityTexture, 52, 16, 16);
            break;
        case 5:
            DrawSpriteSheetSprite(program, p2Score.EntityTexture, 53, 16, 16);
            break;
        case 6:
            DrawSpriteSheetSprite(program, p2Score.EntityTexture, 54, 16, 16);
            break;
        default:
            break;
    }
    
    modelMatrix.identity();
    modelMatrix.Translate(p1Paddle.xPosition, p1Paddle.yPosition, 0.0f);
    program.setModelMatrix(modelMatrix);
    p1Paddle.Draw(program);
    
    modelMatrix.identity();
    modelMatrix.Translate(pongBall.xPosition, pongBall.yPosition, 0.0f);
    program.setModelMatrix(modelMatrix);
    pongBall.Draw(program);
    
    modelMatrix.identity();
    modelMatrix.Translate(p2Paddle.xPosition, p2Paddle.yPosition, 0.0f);
    program.setModelMatrix(modelMatrix);
    p2Paddle.Draw(program);
    
    
}

void setBackgroundColorAndClear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
}

GLuint LoadTexture(const char *image_path)
{
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SDL_FreeSurface(surface);
    
    return textureID;
}


void drawTexture(GLuint theTexture, ShaderProgram program, float height, float width)
{
    float vertices[] = {-width, -height, width, -height, width, height,
        -width, -height, width, height, -width, height};
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    //std::cout << program.positionAttribute << std::endl;
    float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, theTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
}

void initializeEntities()
{

    p1Paddle.EntityTexture = LoadTexture("aPaddle.jpg");
    p1Paddle.speed = 2.4f;
    p1Paddle.height = 0.40f;
    p1Paddle.width = 0.04f;
    p1Paddle.xPosition = -(MaxXPos-(p1Paddle.width*4));
    p1Paddle.yPosition = 0.0f;
    
    pongBall.EntityTexture = LoadTexture("pongBall.png");
    pongBall.speed = 2.8f;
    pongBall.height = 0.08f;
    pongBall.width = 0.08f;
    pongBall.xPosition = 0.0f;
    pongBall.yPosition = 0.0f;
    pongBall.yVelocity = 0.0f;
    pongBall.xVelocity = 0.0f;
    
    p2Paddle.EntityTexture = LoadTexture("aPaddle.jpg");
    p2Paddle.speed = 2.4f;
    p2Paddle.height = 0.40f;
    p2Paddle.width = 0.04f;
    p2Paddle.xPosition = (MaxXPos-(p1Paddle.width*4));
    p2Paddle.yPosition = 0.0f;
    
    p1Score.EntityTexture = LoadTexture("font1.png");
    p1Score.speed = 0.0f;
    p1Score.height = 0.3f;
    p1Score.width = 0.3f;
    p1Score.xPosition = -1.0f;
    p1Score.yPosition = 1.5f;
    
    p2Score.EntityTexture = LoadTexture("font1.png");
    p2Score.speed = 0.0f;
    p2Score.height = 0.3f;
    p2Score.width = 0.3f;
    p2Score.xPosition = 1.0f;
    p2Score.yPosition = 1.5f;
    
}

int DetectCollision(Entity entityOne/* paddle */, Entity entityTwo /*pongBall*/)
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
    
    float entityOneTopThirdBegin = fabs(entityOne.yPosition)+(entityOne.height/6);
    float entityOneBotThirdBegin = fabs(entityOne.yPosition)-(entityOne.height/6);
    
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
        //differenceBetween the absolute value of the two entities yPosition
        //yDifferenceBetweenTwoEntities = fabs(entityOne.yPosition) - fabs(entityTwo.yPosition);
        //if the difference is positive (paddle is higher than ball):
        if(entityOneTopThirdBegin < entityTwo.yPosition + entityTwo.height)
        {
            return 3;
        }
        else if(entityOneBotThirdBegin > entityTwo.yPosition - entityTwo.height)
        {
            return 2;
        }
        else
        {
            return 1;
        }
        //return 1;
    }
    return 0;
    
}

void DrawSpriteSheetSprite(ShaderProgram &program, GLuint theTexture, int index, int spriteCountX, int spriteCountY)
{
    float u = (float)(((int)index) % spriteCountX) / (float) spriteCountX;
    float v = (float)(((int)index) / spriteCountX) / (float) spriteCountY;
    float spriteWidth = 1.0/(float)spriteCountX;
    float spriteHeight = 1.0/(float)spriteCountY;
    GLfloat texCoords[] = {
        u, v+spriteHeight,
        u+spriteWidth, v,
        u, v,
        u+spriteWidth, v,
        u, v+spriteHeight,
        u+spriteWidth, v+spriteHeight
    };
    float vertices[] = {-0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f,  -0.5f,
        -0.5f, 0.5f, -0.5f};
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    //std::cout << program.positionAttribute << std::endl;
    //float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glBindTexture(GL_TEXTURE_2D, theTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
}


