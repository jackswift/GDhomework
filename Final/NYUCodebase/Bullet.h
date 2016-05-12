//
//  Bullet.h
//  NYUCodebase
//
//  Created by Lyle True on 2/22/16.
//  Copyright (c) 2016 Ivan Safrin. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <vector>
#include "LoadAndDrawFuncs.h"



class Bullet
{
public:
    
    Bullet();
    Bullet(float x, float y, float angle, float speed) : x(x), y(y), angle(angle), speed(speed)
    {
        timeAlive = 0.0f;
    }
    
    void Update(float elapsed);
    void Render(ShaderProgram program, Matrix modelMatrix);
   
    
    float x;
    float y;
    
    float width;
    float height;
    
    float angle;
    float speed;
    
    float yVelocity;
    
    float timeAlive;
    
    GLuint textureID;
    
    
private:
    
};


void shootBullet(float x, float y, float yVelocity, std::vector<Bullet> &bullets, bool isPlayer);
bool shouldRemoveBullet(Bullet bullet);


