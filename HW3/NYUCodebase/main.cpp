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
    int column;
    bool isLast;
    
    float xStartPosition;
    float yStartPosition;
    
    
    
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
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if(state == STATE_GAME_LEVEL)
            {
                if(event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                {
                    if(playerBullets.size() <= 3)
                    {
                        shootBullet(vectorOfEnts[0].xPosition, vectorOfEnts[0].yPosition+(vectorOfEnts[0].sprite.height)+0.09f, 1.0f, playerBullets, true);
                    }
                
                }
            }
            else if(state == PLAYER_HIT)
            {
                if(event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                {
                    state = STATE_GAME_LEVEL;
                }
            }
            if(state == STATE_MAIN_MENU)
            {
                if(event.key.keysym.scancode == SDL_SCANCODE_1)
                {
                    state = STATE_GAME_LEVEL;
                }
            }
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
    bool gameOver = false;
    //update player entity to move to the left when "A" is pressed
    if(keys[SDL_SCANCODE_A])
    {
        vectorOfEnts[0].xVelocity = -1.0f;
        vectorOfEnts[0].xPosition += elapsed * (vectorOfEnts[0].xVelocity * vectorOfEnts[0].speed);
        //std::cout << vectorOfEnts[0].speed << std::endl;
        //Make sure player doesn't fly off screen
        if(vectorOfEnts[0].xPosition < (-MaxXPos) + (vectorOfEnts[0].sprite.width+0.05f))
        {
            
            vectorOfEnts[0].speed = 0.0f;
        }
        else
        {
            vectorOfEnts[0].speed = 1.0f;
        }
    }
    //update player entity to move to the right when "D" is pressed
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
    
    //go through each entity in the vectorOfEnts, check if any of these are hit
    //by a bullet
    for(int i = 0; i < vectorOfEnts.size(); i++)
    {
        if(vectorOfEnts.size() <= 11)
        {
            if(i != 0)
            {
                vectorOfEnts[i].speed = 1.0f;
            }
        }
        int v1 = rand() % 1000;
        for(int j = 0; j < playerBullets.size(); j++)
        {
            if(DetectCollisionBullet(vectorOfEnts[i], playerBullets[j]))
            {
                playerScore += 10;
                if(vectorOfEnts[i].isLast)
                {
                    int x = findNextLastEntity(vectorOfEnts, vectorOfEnts[i].column, i);
                    vectorOfEnts[x].isLast = true;
                    //std::cout << "column hit was: " << vectorOfEnts[i].column << " pos of new last = " << x << std::endl;
                }
                vectorOfEnts.erase(vectorOfEnts.begin() + i);
                playerBullets.erase(playerBullets.begin() + j);
            }
        }
        
        //check if the entity is the last of its row:
        if(vectorOfEnts[i].isLast)
        {
            //random number generator to decide if the entity is going
            //to shoot a bullet
            if(v1 <= 5)
            {
                shootBullet(vectorOfEnts[i].xPosition, vectorOfEnts[i].yPosition - (vectorOfEnts[i].sprite.height + 0.09), -1.0f, enemyBullets, false);
            }
            if(vectorOfEnts[i].yPosition <= vectorOfEnts[0].yPosition)
            {
                playerLives--;
                userGetsHit(program, modelMatrix);
            }
        }
    }
    //Check if player gets hit by enemy bullets
    for(int i = 0; i < enemyBullets.size(); i++)
    {
        //FOR NOW: end game if player gets hit
        if(DetectCollisionBullet(vectorOfEnts[0], enemyBullets[i]))
        {
            std::cout << playerLives << std::endl;
            if(playerLives <= 0)
            {
                gameOver = true;
                resetGame(gameOver);
                state = STATE_MAIN_MENU;
                break;
            }
            else
            {
                playerLives--;
            }
            userGetsHit(program, modelMatrix);
            //std::cout << playerLives << std::endl;
        }
    }
    if(vectorOfEnts.size() == 1)
    {
        resetGame(gameOver);
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
    enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(), shouldRemoveBullet), enemyBullets.end());
    for(int i = 0; i < enemyBullets.size(); i++)
    {
        enemyBullets[i].Update(elapsed);
    }
    
    
    std::ostringstream playerScoreConv;
    playerScoreConv << playerScore;
    
    playerScoreStr = playerScoreConv.str();
    
    

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

int findNextLastEntity(std::vector<Entity> entities, int column, int posNotToCheck)
{
    int entPosToReturn = -5000;
    for(int i = 0; i < entities.size(); i++)
    {
        if(i != posNotToCheck)
        {
            if(entities[i].column == column)
            {
                entPosToReturn = i;
            }
        }
    }
    return entPosToReturn;
}

void RenderGameLevel(ShaderProgram program, Matrix modelMatrix)
{
    GLuint background = LoadTexture("darkPurple.png", true);
    GLuint fontSheet = LoadTexture("font1.png", false);
    SheetSprite backgroundSprite(background, 0.0f, 0.0f, 3.55f, 2.0f, 4.0f);
    modelMatrix.identity();
    backgroundSprite.Draw(program);
    for(int i = 0; i < vectorOfEnts.size(); i++)
    {
        modelMatrix.identity();
        modelMatrix.Translate(vectorOfEnts[i].xPosition, vectorOfEnts[i].yPosition, 0.0f);
        program.setModelMatrix(modelMatrix);
        vectorOfEnts[i].Draw(program);
    }
    for(int i = 0; i < playerBullets.size(); i++)
    {
        playerBullets[i].Render(program, modelMatrix);
    }
    for(int i = 0; i < enemyBullets.size(); i++)
    {
        enemyBullets[i].Render(program, modelMatrix);
    }
    std::string score = "Score: ";
    modelMatrix.identity();
    modelMatrix.Translate(-1.25f, MaxYPos - 0.1f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, score, 0.40f, -0.18f);
    
    modelMatrix.identity();
    modelMatrix.Translate(0.0f, MaxYPos - 0.1f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, playerScoreStr, 0.40f, -0.1f);
    
    for(int i = 0; i < playerLives; i++)
    {
        modelMatrix.identity();
        modelMatrix.Translate(-MaxXPos+(vectorOfEnts[0].sprite.width*((i*3)+2)), -MaxYPos+vectorOfEnts[0].sprite.height, 0.0f);
        program.setModelMatrix(modelMatrix);
        vectorOfEnts[0].Draw(program);
    }
    
}
void RenderMainMenu(ShaderProgram program, Matrix modelMatrix)
{
    //Background of game: (purple with stars)
    GLuint fontSheet = LoadTexture("font1.png", false);
    GLuint background = LoadTexture("darkPurple.png", true);
    SheetSprite backgroundSprite(background, 0.0f, 0.0f, 3.55f, 2.0f, 4.0f);
    backgroundSprite.Draw(program);
    
    //SpaceInvaders Text:
    std::string text = "Space Invaders!";
    modelMatrix.identity();
    modelMatrix.Translate(-0.33f*5.1f, 1.7f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, text, 0.35f, -0.1f);
    
    std::string mainMenu = "Main Menu";
    modelMatrix.identity();
    modelMatrix.Translate(-0.3f*4.0f, 1.0f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, mainMenu, 0.38f, -0.1f);
    
    std::string playerOne = "Press 1 for one player";
    modelMatrix.identity();
    modelMatrix.Translate(-0.3f*5.6f, -0.05f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, playerOne, 0.25f, -0.1f);
    
    
}
void RenderPlayerHit(ShaderProgram program, Matrix modelMatrix)
{
    GLuint background = LoadTexture("darkPurple.png", true);
    SheetSprite backgroundSprite(background, 0.0f, 0.0f, 3.55f, 2.0f, 4.0f);
    backgroundSprite.Draw(program);
    
    GLuint fontSheet = LoadTexture("font1.png", false);
    std::string text = "You got hit, you have : ";
    modelMatrix.identity();
    modelMatrix.Translate(-0.35f*5.1f, 0.0f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, text, 0.20f, -0.1f);
    
    std::ostringstream playerLivesConv;
    playerLivesConv << playerLives+1;
    modelMatrix.identity();
    modelMatrix.Translate(0.60f, 0.0f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, playerLivesConv.str(), 0.20f, -0.1f);
    
    text = "live(s) left.";
    modelMatrix.identity();
    modelMatrix.Translate(0.75f, 0.0f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, text, 0.20f, -0.1f);
    
    text = "Press space to continue.";
    modelMatrix.identity();
    modelMatrix.Translate(-0.35f*3.5f, -0.5f, 0.0f);
    program.setModelMatrix(modelMatrix);
    DrawText(&program, fontSheet, text, 0.20f, -0.1f);
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
    GLuint SheetTextures = LoadTexture("sheet.png", false);
    Entity playerEnt;
    //x="224" y="832" width="99" height="75"
    playerEnt.sprite = SheetSprite(SheetTextures, 224.0f/1024.0f, 832.0f/1024.0f, 99.0f/1024.0f, 75.0f/1024.0f, 0.2);
    playerEnt.xVelocity = 0.0f;
    playerEnt.yVelocity = 0.0f;
    playerEnt.usesSprite = true;
    playerEnt.rotation = 0.0f;
    playerEnt.xPosition = 0.0f;
    playerEnt.speed = 0.85f;
    playerEnt.yPosition = -MaxYPos + 0.25f;
    playerEnt.xStartPosition = 0.0f;
    playerEnt.yStartPosition = -MaxYPos + 0.25f;
    playerEnt.usesSprite = true;
    playerEnt.isLast = false;
    playerEnt.column = -5000;
    playerEnt.height = 0.0f;
    playerEnt.width = 0.0f;
    playerEnt.EntityTexture = SheetTextures;
    
    vectorOfEnts.push_back(playerEnt);
    
    Entity enemyEnt;
    
    float x = 0.44f + 0.44f + (-MaxXPos);
    float y = MaxYPos - 0.5f;
    float tempX = x;
    enemyEnt.sprite = SheetSprite(SheetTextures, 425.0f/1024.0f, 468.0f/1024.0f, 93.0f/1024.0f, 84.0f/1024.0f, 0.2);
    enemyEnt.xVelocity = 1.0f;
    enemyEnt.yVelocity = 0.0f;
    enemyEnt.usesSprite = true;
    enemyEnt.rotation = 0.0f;
    enemyEnt.xPosition = tempX;
    enemyEnt.speed = 0.35f;
    enemyEnt.yPosition = y;
    enemyEnt.xStartPosition = tempX;
    enemyEnt.yStartPosition = y;
    enemyEnt.usesSprite = true;
    enemyEnt.isLast = false;
    enemyEnt.height = 0.0f;
    enemyEnt.width = 0.0f;
    enemyEnt.column = 0;
    enemyEnt.EntityTexture = SheetTextures;
    vectorOfEnts.push_back(enemyEnt);
    //First enemy's x: -2.6628 First enemy's y: 1.5
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
        enemyEnt.xStartPosition = tempX;
        enemyEnt.yStartPosition = y;
        enemyEnt.usesSprite = true;
        enemyEnt.speed = 0.35f;
        enemyEnt.EntityTexture = SheetTextures;
        enemyEnt.column = i % 11;
        //std::cout << "enemyEnt i = " << i << " column number = " << enemyEnt.column << std::endl;
        //std::cout << "Does anything work here?" << std::endl;
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
 
}
void userGetsHit(ShaderProgram program, Matrix modelMatrix)
{
    long enemyBulletsSize = enemyBullets.size();
    for(int i = 0; i < enemyBulletsSize; i++)
    {
        enemyBullets.pop_back();
    }
    long playerBulletsSize = playerBullets.size();
    for(int i = 0; i < playerBulletsSize; i++)
    {
        playerBullets.pop_back();
    }
    for (int i = 0; i < vectorOfEnts.size(); i++)
    {
        vectorOfEnts[i].xPosition = vectorOfEnts[i].xStartPosition;
        vectorOfEnts[i].yPosition = vectorOfEnts[i].yStartPosition;
    }
    state = PLAYER_HIT;
    
}
void resetGame(bool gameOver)
{
    long vectorOfEntsSize = vectorOfEnts.size();
    if(gameOver)
    {
        playerLives = 2;
        playerScore = 0;
    }
    for(int i = 0; i < vectorOfEntsSize; i++)
    {
        vectorOfEnts.pop_back();
    }
    long enemyBulletsSize = enemyBullets.size();
    for(int i = 0; i < enemyBulletsSize; i++)
    {
        enemyBullets.pop_back();
    }
    long playerBulletsSize = playerBullets.size();
    for(int i = 0; i < playerBulletsSize; i++)
    {
        playerBullets.pop_back();
    }
    initializeEntities();
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



