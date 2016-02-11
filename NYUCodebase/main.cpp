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

GLuint LoadTexture(const char *image_path);
void setBackgroundColorAndClear();
void setProgramMatrices(ShaderProgram &program, Matrix projectionMatrix, Matrix modelMatrix, Matrix viewMatrix);
SDL_Window* displayWindow;

void drawTexture(GLuint texture, ShaderProgram program);


int main(int argc, char *argv[])
{
    /*
     * Initialize the window, video, and context. Not sure what makeCurrent does, but I'm assuming
     * that it changes the window that I'm currently viewing to the window I should be seeing.
     */
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 360);
    //Setup program and initialize matrixes
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    
    
    GLuint smileyFace = LoadTexture("Smiley.png");
    GLuint kittenPic = LoadTexture("kitten.png");
    GLuint someImg = LoadTexture("someImg.png");
    
    Matrix projectionMatrix;
    Matrix modelMatrix;
    //Matrix modelMatrix2;
    //Matrix modelMatrix3;
    Matrix viewMatrix;
    
    //positions of textures
    float lastFrameticks = 0.0f;
    float smileyX = 1.0f;
    float smileyY = 0.0f;
    float kittenX = -1.0f;
    float kittenY = 0.0f;
    float someImgX = 0.0f;
    float someImgY = 0.0f;
    float someImgRotation = 0.0f;
    //set ortho projection of the projectionMatrix (changing these values will set the cartesian
    //plane units of the game screen
    projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        float ticks = (float)SDL_GetTicks() / 1000.0f;
        float elapsed = ticks - lastFrameticks;
        lastFrameticks = ticks;
        //setProgramMatrices(program, projectionMatrix, modelMatrix, viewMatrix);
        
        smileyX += elapsed;
        if(smileyX >= (MaxXPos-0.5f))
        {
            smileyX = 1.0f;
        }
        kittenX -= elapsed;
        if(kittenX <= (-MaxXPos+0.5f))
        {
            kittenX = -1.0f;
        }
        someImgY += elapsed*2;
        if(someImgY >= (MaxYPos-0.5f))
        {
            someImgY = -1.5f;
        }
        someImgRotation += elapsed*80;
        
        
        setBackgroundColorAndClear();
        
        program.setModelMatrix(modelMatrix);
        program.setViewMatrix(viewMatrix);
        program.setProjectionMatrix(projectionMatrix);
        
        modelMatrix.identity();
        modelMatrix.Translate(smileyX, smileyY, 0.0f);
        program.setModelMatrix(modelMatrix);
        drawTexture(smileyFace, program);
        
        modelMatrix.identity();
        modelMatrix.Translate(kittenX, kittenY, 0.0f);
        program.setModelMatrix(modelMatrix);
        drawTexture(kittenPic, program);
        
        modelMatrix.identity();
        modelMatrix.Translate(someImgX, someImgY, 0.0f);
        modelMatrix.Rotate(someImgRotation * (3.14159/180.0f));
        program.setModelMatrix(modelMatrix);
        drawTexture(someImg, program);
        
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
    
    glBindTexture(GL_TEXTURE_2D, theTexture);
    float vertices[] = {-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f};
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    //std::cout << program.positionAttribute << std::endl;
    float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
}
