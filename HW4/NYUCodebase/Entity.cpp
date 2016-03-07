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

void Entity::UpdateX(float elapsed)
{
    //Do x updates
    xVelocity = lerp(xVelocity, 0.0f, elapsed * xFriction);
    xVelocity += acceleration_x * elapsed;
    xPosition += (xVelocity) * elapsed;
    
    //Do y updates
    
}
void Entity::UpdateY(float elapsed)
{
    yVelocity += acceleration_y * elapsed;
    yVelocity += gravity_y * elapsed;
    yPosition += (yVelocity) * elapsed;
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