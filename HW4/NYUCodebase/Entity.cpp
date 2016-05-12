//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Lyle True on 3/5/16.
//  Copyright (c) 2016 Ivan Safrin. All rights reserved.
//

#include "Entity.h"
#include "LoadAndDrawFuncs.h"


float lerp(float v0, float v1, float t)
{
    return (1.0-t)*v0 + t*v1;
}

int Entity::collidesWith(Entity *entity)
{

    float entOneLeft = 0.0f;
    //x position of the right hand side of entityOne
    float entOneRight = 0.0f;
    //y position of the top of entityOne
    float entOneTop = 0.0f;
    //y position of the bottom of entityOne
    float entOneBot = 0.0f;
    if(entity->entityType == ENTITY_ICICLE)
    {
        entOneLeft = entity->entityVector.x-entity->width/2;
        //x position of the right hand side of entityOne
        entOneRight = entity->entityVector.x+entity->width/2;
        //y position of the top of entityOne
        entOneTop = entity->entityVector.y+entity->height/2;
        //y position of the bottom of entityOne
        entOneBot = entity->entityVector.y;
    }
    else if(entity->entityType == ENTITY_PLATFORM)
    {
        //vectorOfEnts[index].entityVector.y - vectorOfEnts[index].height/2 + 0.057
        entOneLeft = entity->entityVector.x-entity->width/2;
        //x position of the right hand side of entityOne
        entOneRight = entity->entityVector.x+entity->width/2;
        //y position of the top of entityOne
        entOneTop = entity->entityVector.y - entity->height/2 + 0.057;
        //y position of the bottom of entityOne
        entOneBot = entity->entityVector.y - entity->height/2;
    }
    else
    {
        entOneLeft = entity->entityVector.x-entity->width/2;
        //x position of the right hand side of entityOne
        entOneRight = entity->entityVector.x+entity->width/2;
        //y position of the top of entityOne
        entOneTop = entity->entityVector.y+entity->height/2;
        //y position of the bottom of entityOne
        entOneBot = entity->entityVector.y-entity->height/2;
    }
    //x position of the left hand side of entityTwo
    float entTwoLeft = this->entityVector.x-this->width/2;
    //x position of the right hand side of entityTwo
    float entTwoRight = this->entityVector.x+this->width/2;
    //y position of the top of entityTwo
    float entTwoTop = this->entityVector.y+this->height/2;
    //y position of the bot of entityTwo
    float entTwoBot = this->entityVector.y-this->height/2;
    
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
        return 1;
    }
    else
    {
        return 0;
    }
}
//Do y updates
void Entity::UpdateX(float elapsed)
{
    xVelocity = lerp(xVelocity, 0.0f, elapsed * xFriction);
    xVelocity += acceleration_x * elapsed;
    entityVector.x += (xVelocity) * elapsed;
}

//Do y updates
void Entity::UpdateY(float elapsed)
{
    //yVelocity += acceleration_y * elapsed;
    yVelocity += gravity_y * elapsed;
    entityVector.y += (yVelocity) * elapsed;
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

void Entity::setSprite(int spriteCountY, int spriteCountX, int index, float size)
{
    float u = (float)(((int)index) % spriteCountX) / (float) spriteCountX;
    float v = (float)(((int)index) / spriteCountX) / (float) spriteCountY;
    float spriteWidth = 1.0/(float)spriteCountX;
    float spriteHeight = 1.0/(float)spriteCountY;
    this->sprite = SheetSprite(this->EntityTexture, u, v, spriteWidth, spriteHeight, size);
    
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