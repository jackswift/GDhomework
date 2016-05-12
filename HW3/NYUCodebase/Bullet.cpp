//
//  Bullet.cpp
//  NYUCodebase
//
//  Created by Lyle True on 2/22/16.
//  Copyright (c) 2016 Ivan Safrin. All rights reserved.
//

#include "Bullet.h"



Bullet::Bullet()
{
    
}
void Bullet::Update(float elapsed)
{
    x += yVelocity * elapsed * speed;
    timeAlive += elapsed;
}

void Bullet::Render(ShaderProgram program, Matrix modelMatrix)
{
    modelMatrix.identity();
    modelMatrix.Translate(x, y, 0.0f);
    program.setModelMatrix(modelMatrix);
    drawTexture(textureID, program, height, width);
}
//<SubTexture name="fire09.png" x="811" y="663" width="16" height="40"/>

void shootBullet(float x, float y, float yVelocity, std::vector<Bullet> &bullets, bool isPlayer)
{
    Bullet newBullet;
    if(isPlayer)
    {
        GLuint texture = LoadTexture("laserGreen03.png", false);
        newBullet.textureID = texture;
    }
    else
    {
        GLuint texture = LoadTexture("laserRed03.png", false);
        newBullet.textureID = texture;
    }
    newBullet.x = x;
    newBullet.y = y;
    newBullet.height = 0.03f;
    newBullet.width = 0.015f;
    newBullet.yVelocity = yVelocity;
    if(isPlayer)
    {
        newBullet.speed = 3.5f;
    }
    else
    {
        newBullet.speed = 2.3f;
    }
    
    newBullet.timeAlive = 0.0f;
    bullets.push_back(newBullet);
}

bool shouldRemoveBullet(Bullet bullet)
{
    if(bullet.timeAlive > 1.8f)
    {
        return true;
    }
    else
    {
        return false;
    }
}
