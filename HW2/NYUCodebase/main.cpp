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
void setProgramMatrices(ShaderProgram &program, Matrix projectionMatrix, Matrix modelMatrix, Matrix viewMatrix);
void drawTexture(GLuint theTexture, ShaderProgram program, float height = 0.5f, float width = 0.5f);
void Setup(Matrix &projectionMatrix);

void ProcessEvents();

void Update(Matrix &modelMatrix, float lastFrameTicks);
void Render(Matrix &modelMatrix, ShaderProgram program);

void initializeEntities();

class Entity
{
public:
    //Entity(const char* imageName, float xPosition = 0.0f, float yPosition = 0.0f, float xVelocity = 0.0f, float yVelocity = 0.0f) : xPosition(xPosition), yPosition(yPosition), xVelocity(xVelocity), yVelocity(yVelocity)
    //{
    //    EntityTexture = LoadTexture(imageName);
    //}
    
    //void Render(ShaderProgram program);
    
    //void Update(float elapsed);
    
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

/*
void Entity::Update(float elapsed)
{
    xPosition += xVelocity * elapsed;
    if(xPosition >= (MaxXPos-0.5f))
    {
        xPosition = -MaxXPos+0.5f;
    }
    yPosition += yVelocity * elapsed;
    if(yPosition <= (-(MaxYPos-0.5f)))
    {
        yPosition = MaxYPos-0.5f;
    }
    EntityModelMatrix.identity();
    EntityModelMatrix.Translate(xPosition, yPosition, 0.0f);
}
*/

Entity p1Paddle;
Entity pongBall;
int main(int argc, char *argv[])
{
    srand(time(0));
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    Setup(projectionMatrix);
    
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
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
        
        Update(modelMatrix, lastFrameTicks);
        
        Render(modelMatrix, program);
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}

void Setup(Matrix &projectionMatrix)
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

void ProcessEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
    }

}

void Update(Matrix &modelMatrix, float lastFrameTicks)
{
    /* PADDLE 1 MOVEMENT
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
     
    modelMatrix.identity();
    modelMatrix.Translate(p1Paddle.xPosition, p1Paddle.yPosition, 0.0f);
     */
    if(keys[SDL_SCANCODE_SPACE] && pongBall.xVelocity == 0.0f && pongBall.yVelocity == 0.0f)
    {
        pongBall.xVelocity = (float(rand() % 100 + 1))/100;
        pongBall.yVelocity = 0.90f;
    }
    pongBall.xPosition += elapsed * (pongBall.speed * pongBall.xVelocity);
    pongBall.yPosition += elapsed * (pongBall.speed * pongBall.yVelocity);
    if(pongBall.xPosition > MaxXPos-pongBall.width || pongBall.xPosition < (-MaxXPos)+pongBall.width)
    {
        pongBall.xVelocity = -pongBall.xVelocity;
    }
    if(pongBall.yPosition > MaxYPos-pongBall.height || pongBall.yPosition < (-MaxYPos)+pongBall.height)
    {
        pongBall.yVelocity = -pongBall.yVelocity;
    }
    modelMatrix.identity();
    modelMatrix.Translate(pongBall.xPosition, pongBall.yPosition, 0.0f);
    
    
}

void Render(Matrix &modelMatrix, ShaderProgram program)
{
    p1Paddle.Draw(program);
    pongBall.Draw(program);
    
}
void setBackgroundColorAndClear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
}
void setProgramMatrices(ShaderProgram &program, Matrix projectionMatrix, Matrix modelMatrix, Matrix viewMatrix)
{
    program.setModelMatrix(modelMatrix);
    program.setViewMatrix(viewMatrix);
    program.setProjectionMatrix(projectionMatrix);
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
    /* PADDLE 1 INITIATION
    p1Paddle.EntityTexture = LoadTexture("aPaddle.jpg");
    p1Paddle.speed = 2.4f;
    p1Paddle.height = 0.55f;
    p1Paddle.width = 0.04f;
    p1Paddle.xPosition = -(MaxXPos-(p1Paddle.width*4));
    p1Paddle.yPosition = 0.0f;
     */
    pongBall.EntityTexture = LoadTexture("pongBall.png");
    pongBall.speed = 3.3f;
    pongBall.height = 0.08f;
    pongBall.width = 0.08f;
    pongBall.xPosition = 0.0f;
    pongBall.yPosition = 0.0f;
    pongBall.yVelocity = 0.0f;
    pongBall.xVelocity = 0.0f;
    //std::cout << pongBall.xVelocity << std::endl;
    //std::cout << pongBall.yVelocity << std::endl;
    
}

bool DetectCollision(Entity entityOne, Entity entityTwo)
{
    //Box to box collision detection:
    /*
     a) is R1’s bottom higher than R2’s top? 
     b) is R1’s top lower than R2’s bottom? 
     c) is R1’s left larger than R2’s right?
     d) is R1’s right smaller than R2’s left
     
     The rectangles are intersecting if NONE of the above are true.
     */
    float entOneLeft = entityOne.xPosition-entityOne.width;
    float entOneRight = entityOne.xPosition+entityOne.width;
    float entOneTop = entityOne.yPosition+entityOne.height;
    float entOneBot = entityOne.yPosition-entityOne.height;
    
    float entTwoLeft = entityTwo.xPosition-entityTwo.width;
    float entTwoRight = entityTwo.xPosition+entityTwo.width;
    float entTwoTop = entityTwo.yPosition+entityTwo.height;
    float entTwoBot = entityTwo.yPosition-entityTwo.height;
    
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

