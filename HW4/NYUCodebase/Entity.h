//
//  Entity.h
//  NYUCodebase
//
//  Created by Lyle True on 3/5/16.
//  Copyright (c) 2016 Ivan Safrin. All rights reserved.
//

#pragma once
#include "ShaderProgram.h"


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

enum EntityType {ENTITY_PLAYER, ENTITY_ENEMY, ENTITY_COIN, ENTITY_BLOCK};
class Entity
{
public:
    void Draw(ShaderProgram program);
    void UpdateX(float elapsed);
    void UpdateY(float elapsed);
    
    bool collidesWith(Entity *entity);
    
    float xPosition;
    float yPosition;
    float rotation = 0;
    
    float height;
    float width;
    
    float xVelocity = 0;
    float yVelocity = 0;
    float xFriction;
    
    
    float acceleration_x = 0;
    float acceleration_y = 0;
    float gravity_y;
    
    
    GLuint EntityTexture;
    SheetSprite sprite;
    bool usesSprite;
    void setSprite(int spriteCountY, int spriteCountX, int index, float size);
    
    bool isStatic;
    EntityType entityType;
    
    bool collidedTop = false;
    bool collidedBot = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    
    //Matrix EntityModelMatrix;
    //float xAcceleration;
    //float yAcceleration;
private:
    
};
