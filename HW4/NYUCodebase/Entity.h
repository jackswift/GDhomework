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

enum EntityType {ENTITY_PLAYER, ENTITY_ENEMY, ENTITY_COIN};
class Entity
{
public:
    void Draw(ShaderProgram program);
    void UpdateX(float elapsed);
    void UpdateY(float elapsed);
    
    bool collidesWith(Entity *entity);
    
    float xPosition;
    float yPosition;
    float rotation;
    
    float height;
    float width;
    
    float xVelocity;
    float yVelocity;
    float xFriction;
    
    
    float acceleration_x;
    float acceleration_y;
    float gravity_y;
    
    
    GLuint EntityTexture;
    SheetSprite sprite;
    bool usesSprite;
    
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
