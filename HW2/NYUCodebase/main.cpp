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

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path);
void setBackgroundColorAndClear();
void setProgramMatrices(ShaderProgram &program, Matrix projectionMatrix, Matrix modelMatrix, Matrix viewMatrix);
void drawTexture(GLuint theTexture, ShaderProgram program);



class Sprite
{
public:
    Sprite(Matrix modelMatrix, const char* imageName, float xPosition = 0.0f, float yPosition = 0.0f, float xVelocity = 0.0f, float yVelocity = 0.0f) : spriteModelMatrix(modelMatrix), xPosition(xPosition), yPosition(yPosition), xVelocity(xVelocity), yVelocity(yVelocity)
    {
        spriteTexture = LoadTexture(imageName);
    }
    
    void Render(ShaderProgram program);
    
    void Update(float elapsed);
    
private:
    float xPosition;
    float yPosition;
    float xVelocity;
    float yVelocity;
    GLuint spriteTexture;
    Matrix spriteModelMatrix;
    //float xAcceleration;
    //float yAcceleration;
    
};

void Sprite::Render(ShaderProgram program)
{
    program.setModelMatrix(spriteModelMatrix);
    drawTexture(spriteTexture, program);
    
}

void Sprite::Update(float elapsed)
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
    spriteModelMatrix.identity();
    spriteModelMatrix.Translate(xPosition, yPosition, 0.0f);
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    
    //Program and window stuff
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    glViewport(0, 0, 640, 360);
    
    //Animation, Matrix, and Sprites variables
    float lastFrameTicks = 0.0f;
    Matrix modelMatrix;
    Matrix sprite1ModelMatrix;
    Matrix viewMatrix;
    Matrix projectionMatrix;
    float sprite1XPos = 0.0f;
    float sprite1YPos = 0.0f;
    float sprite1XVelo = 1.0f;
    float sprite1YVelo = -1.0f;
    Sprite p1Sprite(sprite1ModelMatrix, "p2_stand.png", sprite1XPos, sprite1YPos, sprite1XVelo, sprite1YVelo);
    
    
    //initialize and set the 'end of program' boolean to false. Also initialize
    //variable to catch events.
    SDL_Event event;
    bool done = false;
    
    //set the orthographic projection
    projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
    
    //enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        float ticks = (float)SDL_GetTicks() / 1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        p1Sprite.Update(elapsed);
        
        program.setModelMatrix(modelMatrix);
        program.setViewMatrix(viewMatrix);
        program.setProjectionMatrix(projectionMatrix);
        
        setBackgroundColorAndClear();
        
        glUseProgram(program.programID);
        
        p1Sprite.Render(program);
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}


void setBackgroundColorAndClear()
{
    glClearColor(255.0f, 0.0f, 0.0f, 0.0f);
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


void drawTexture(GLuint theTexture, ShaderProgram program)
{
    //Matrix modelMatrix;
    //Matrix viewMatrix;
    
    
    
    //modelMatrix.identity();
    //modelMatrix.Translate(x, y, 0.0f);
    
    float vertices[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f};
    
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

