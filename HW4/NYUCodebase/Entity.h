//
//  Entity.h
//  NYUCodebase
//
//  Created by Lyle True on 3/5/16.
//  Copyright (c) 2016 Ivan Safrin. All rights reserved.
//

#pragma once
#include "ShaderProgram.h"
#include "Vector.h"

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

enum EntityType {ENTITY_PLAYER, ENTITY_ICICLE, ENTITY_FIST, ENTITY_ROTATER, ENTITY_PLATFORM, LEVEL_GOAL, DEATH_LOLLI, TURRET};
class Entity
{
public:
    void Draw(ShaderProgram program);
    void UpdateX(float elapsed);
    void UpdateY(float elapsed);
    
    int collidesWith(Entity *entity);
    /*
    void entityCollisionBot(float elapsed, Matrix &modelMatrix, ShaderProgram program, int i);
    void entityCollisionTop(float elapsed, Matrix &modelMatrix, ShaderProgram program, int i);
    void entityCollisionLeft(float elapsed, Matrix &modelMatrix, ShaderProgram program, int i);
    void entityCollisionRight(float elapsed, Matrix &modelMatrix, ShaderProgram program, int i);
     */
    
    //float xPosition;
    //float yPosition;
    float rotation = 1.0;
    
    float height;
    float width;
    
    float xVelocity = 0;
    float yVelocity = 0;
    float xFriction;
    
    
    float acceleration_x = 0.0f;
    float acceleration_y = 0.0f;
    float gravity_y;
    
    
    GLuint EntityTexture;
    SheetSprite sprite;
    bool usesSprite;
    void setSprite(int spriteCountY, int spriteCountX, int index, float size);
    
    bool isStatic;
    EntityType entityType;
    
    Vector entityVector;
    
    bool collidedTop = false;
    bool collidedBot = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    
    /* only for fist entity*/
    bool waiting = false;
    bool falling = false;
    bool returning = false;
    bool fallen = false;
    float timeFallen = 0.0f;
    
    float startingPosition = -100.0f;
    
    //Matrix EntityModelMatrix;
    //float xAcceleration;
    //float yAcceleration;
private:
    
};
