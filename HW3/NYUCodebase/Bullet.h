//
//  Bullet.h
//  NYUCodebase
//
//  Created by Lyle True on 2/22/16.
//  Copyright (c) 2016 Ivan Safrin. All rights reserved.
//

#pragma once

class Bullet
{
public:
    
    Bullet();
    Bullet(float x, float y, float angle, float speed) : x(x), y(y), angle(angle), speed(speed)
    {
        timeAlive = 0.0f;
    }
    void Update();
    void Render();
    
    float x;
    float y;
    
    
    float angle;
    float speed;
    
    float timeAlive;
    
    
private:
    
};