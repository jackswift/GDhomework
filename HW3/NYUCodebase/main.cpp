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

float MaxYPos = 2.0f;
float MaxXPos = 3.55f;
bool done = false;

std::vector<Bullet> playerBullets;

std::vector<Bullet> enemyBullets;
//float ticks;
//float lastFrameTicks = 0.0f;
//float elapsed;


const Uint8 *keys = SDL_GetKeyboardState(NULL);

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path, bool isRepeating);
void setBackgroundColorAndClear();
void drawTexture(GLuint theTexture, ShaderProgram program, float height = 0.5f, float width = 0.5f);
void Setup(Matrix &projectionMatrix);

void ProcessEvents();

void Update(float elapsed, Matrix &modelMatrix, ShaderProgram program);
void Render(ShaderProgram program, Matrix modelMatrix);
void DrawspriteSprite(ShaderProgram &program, GLuint theTexture, int index, int SpriteXCount, int SpriteYCount);

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
    int column;
    bool isLast;
    
    
    
    GLuint EntityTexture;
    SheetSprite sprite;
    bool usesSprite;
    //Matrix EntityModelMatrix;
    //float xAcceleration;
    //float yAcceleration;
private:
    
};

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
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    Setup(projectionMatrix);
    float ticks;
    float lastFrameTicks = 0.0f;
    float elapsed;
    
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
        
        //std::cout << ticks << std::endl;
        
        glUseProgram(program.programID);
        //p1Entity.Update(elapsed);
        setBackgroundColorAndClear();
        program.setModelMatrix(modelMatrix);
        program.setViewMatrix(viewMatrix);
        program.setProjectionMatrix(projectionMatrix);
        
        Update(elapsed, modelMatrix, program);
        
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
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.scancode == SDL_SCANCODE_SPACE)
            {
                if(playerBullets.size() <= 3)
                {
                    shootBullet(vectorOfEnts[0].xPosition, vectorOfEnts[0].yPosition+(vectorOfEnts[0].sprite.height)+0.06f, 1.0f, playerBullets, true);
                }
                
            }
        }
    }

}

void Update(float elapsed, Matrix &modelMatrix, ShaderProgram program)
{
    if(keys[SDL_SCANCODE_A])
    {
        vectorOfEnts[0].xVelocity = -1.0f;
        vectorOfEnts[0].xPosition += elapsed * (vectorOfEnts[0].xVelocity * vectorOfEnts[0].speed);
        //std::cout << vectorOfEnts[0].speed << std::endl;
        if(vectorOfEnts[0].xPosition < (-MaxXPos) + (vectorOfEnts[0].sprite.width+0.05f))
        {
            
            vectorOfEnts[0].speed = 0.0f;
        }
        else
        {
            vectorOfEnts[0].speed = 1.0f;
        }
    }
    else if(keys[SDL_SCANCODE_D])
    {
        vectorOfEnts[0].xVelocity = 1.0f;
        vectorOfEnts[0].xPosition += elapsed * (vectorOfEnts[0].xVelocity * vectorOfEnts[0].speed);
        if(vectorOfEnts[0].xPosition > (MaxXPos) - (vectorOfEnts[0].sprite.width+0.05f))
        {
            vectorOfEnts[0].speed = 0.0f;
        }
        else
        {
            vectorOfEnts[0].speed = 1.0f;
        }
    }
    for(int i = 0; i < vectorOfEnts.size(); i++)
    {
        for(int j = 0; j < playerBullets.size(); j++)
        {
            if(DetectCollisionBullet(vectorOfEnts[i], playerBullets[j]))
            {
                //std::cout << "bullet j: " << j << " hit entity i: " << i << std::endl;
                if(i != 0)
                {
                    vectorOfEnts.erase(vectorOfEnts.begin() + i);
                    playerBullets.erase(playerBullets.begin() + j);
                }
               
            }
        }
    }
    for(int i = 1; i < vectorOfEnts.size(); i++)
    {
        if(vectorOfEnts[i].xPosition > (MaxXPos - (vectorOfEnts[i].width+0.1)) ||
               vectorOfEnts[i].xPosition < ((-MaxXPos) + (vectorOfEnts[i].width+0.1)))
        {
            for(int i = 1; i < vectorOfEnts.size(); i++)
            {
                if(vectorOfEnts[i].xPosition > 0)
                {
                       vectorOfEnts[i].xPosition -= 0.01f;
                }
                else{
                    vectorOfEnts[i].xPosition += 0.01f;
                }
                vectorOfEnts[i].xVelocity = -vectorOfEnts[i].xVelocity;
                vectorOfEnts[i].yPosition -= 0.1;
            }
        }
        vectorOfEnts[i].Update(elapsed);
    }
    playerBullets.erase(std::remove_if(playerBullets.begin(), playerBullets.end(), shouldRemoveBullet), playerBullets.end());
    for(int i = 0; i < playerBullets.size(); i++)
    {
        playerBullets[i].Update(elapsed);
    }
    
}
void Render(ShaderProgram program, Matrix modelMatrix)
{
    GLuint background = LoadTexture("darkPurple.png", true);
    SheetSprite backgroundSprite(background, 0.0f, 0.0f, 3.55f, 2.0f, 4.0f);
    modelMatrix.identity();
    backgroundSprite.Draw(program);
    for(int i = 0; i < vectorOfEnts.size(); i++)
    {
        //std::cout << "i = " << i << " x = " << vectorOfEnts[i].xPosition << " y = " << vectorOfEnts[i].yPosition << std::endl;
        modelMatrix.identity();
        modelMatrix.Translate(vectorOfEnts[i].xPosition, vectorOfEnts[i].yPosition, 0.0f);
        program.setModelMatrix(modelMatrix);
        vectorOfEnts[i].Draw(program);
    }
    for(int i = 0; i < playerBullets.size(); i++)
    {
        playerBullets[i].Render(program, modelMatrix);
    }
}
void setBackgroundColorAndClear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
}
void initializeEntities()
{
    GLuint SheetTextures = LoadTexture("sheet.png", false);
    Entity playerEnt;
    //x="224" y="832" width="99" height="75"
    playerEnt.sprite = SheetSprite(SheetTextures, 224.0f/1024.0f, 832.0f/1024.0f, 99.0f/1024.0f, 75.0f/1024.0f, 0.2);
    playerEnt.xVelocity = 0.0f;
    playerEnt.yVelocity = 0.0f;
    playerEnt.usesSprite = true;
    playerEnt.rotation = 0.0f;
    playerEnt.xPosition = 0.0f;
    playerEnt.speed = 1.0f;
    playerEnt.yPosition = -MaxYPos + 0.15f;
    playerEnt.usesSprite = true;
    vectorOfEnts.push_back(playerEnt);
    
    Entity enemyEnt;
    
    float x = 0.44f + 0.44f + (-MaxXPos);
    float y = MaxYPos - 0.28f;
    float tempX = x;
    enemyEnt.sprite = SheetSprite(SheetTextures, 425.0f/1024.0f, 468.0f/1024.0f, 93.0f/1024.0f, 84.0f/1024.0f, 0.2);
    enemyEnt.xVelocity = 1.0f;
    enemyEnt.yVelocity = 0.0f;
    enemyEnt.usesSprite = true;
    enemyEnt.rotation = 0.0f;
    enemyEnt.xPosition = tempX;
    enemyEnt.speed = 0.45f;
    enemyEnt.yPosition = y;
    enemyEnt.usesSprite = true;
    enemyEnt.isLast = false;
    enemyEnt.column = 0;
    vectorOfEnts.push_back(enemyEnt);
    for(int i = 1; i < 55; i++)
    {
        tempX = x + ((i % 11) * 0.50);
        if(i % 11 == 0)
        {
            y -= 0.28;
        }
        enemyEnt.sprite = SheetSprite(SheetTextures, 425.0f/1024.0f, 468.0f/1024.0f, 93.0f/1024.0f, 84.0f/1024.0f, 0.2);
        enemyEnt.xVelocity = 1.0f;
        enemyEnt.yVelocity = 0.0f;
        enemyEnt.usesSprite = true;
        enemyEnt.rotation = 0.0f;
        enemyEnt.xPosition = tempX;
        enemyEnt.yPosition = y;
        enemyEnt.usesSprite = true;
        enemyEnt.speed = 0.45f;
        enemyEnt.column = i % 11;
        std::cout << i << "'s column = " << enemyEnt.column << std::endl;
        if(i >= 44)
        {
            enemyEnt.isLast = true;
        }
        else
        {
            enemyEnt.isLast = false;
        }
        vectorOfEnts.push_back(enemyEnt);
    }
    //std::cout << "size = " << vectorOfEnts.size() << std::endl;
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



