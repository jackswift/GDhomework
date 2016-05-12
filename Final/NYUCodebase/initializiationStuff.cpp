//
//  initializiationStuff.cpp
//  NYUCodebase
//
//  Created by Lyle True on 5/9/16.
//  Copyright (c) 2016 Ivan Safrin. All rights reserved.
//

#include "initializiationStuff.h"



void initializeEntitiesLevel1(std::vector<Entity> &vectorOfEnts)
{
    vectorOfEnts.clear();
    initializePlayer(vectorOfEnts, 1);
    initializeIcicles(vectorOfEnts, 1);
    initializeFists(vectorOfEnts, 1);
    initializePlatform(vectorOfEnts, 1);
    initializeLollis(vectorOfEnts, 1);
    initializeEndOfLevel(vectorOfEnts, 1);
}
void initializeEntitiesLevel2(std::vector<Entity> &vectorOfEnts)
{
    vectorOfEnts.clear();
    initializePlayer(vectorOfEnts, 2);
    initializeIcicles(vectorOfEnts, 2);
    initializeFists(vectorOfEnts, 2);
    initializePlatform(vectorOfEnts, 2);
    initializeLollis(vectorOfEnts, 2);
    initializeTurrets(vectorOfEnts, 2);
    initializeEndOfLevel(vectorOfEnts, 2);
}

void initializePlayer(std::vector<Entity> &vectorOfEnts, int level)
{
    int index = 19;
    float u = (float)(((int)index) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
    float v = (float)(((int)index) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
    float spriteWidth = 1.0/(float)SPRITE_COUNT_X;
    float spriteHeight = 1.0/(float)SPRITE_COUNT_Y;
    GLuint playerTexture = LoadTexture("spritesheet.png", false);
    if(level == 1)
    {
        Entity playerEntity;
        playerEntity.entityType = ENTITY_PLAYER;
        playerEntity.EntityTexture = playerTexture;
        playerEntity.xVelocity = 0;
        playerEntity.usesSprite = true;
        playerEntity.sprite = SheetSprite(playerTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        playerEntity.height = 0.17f;
        playerEntity.width = 0.17f;
        playerEntity.isStatic = false;
        playerEntity.gravity_y = -3.0f;
        playerEntity.acceleration_x = 0.0f;
        playerEntity.acceleration_y = 0.0f;
        playerEntity.xFriction = 3.0f;
        playerEntity.entityVector.x = 0.342696;
        playerEntity.entityVector.y = -4.30;
        playerEntity.entityVector.z = 0.0f;
        playerEntity.waiting = false;
        playerEntity.falling = false;
        playerEntity.returning = false;
        playerEntity.fallen = false;
        vectorOfEnts.push_back(playerEntity);
    }
    if(level == 2)
    {
        Entity playerEntity;
        playerEntity.entityType = ENTITY_PLAYER;
        playerEntity.EntityTexture = playerTexture;
        playerEntity.xVelocity = 0;
        playerEntity.usesSprite = true;
        playerEntity.sprite = SheetSprite(playerTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        playerEntity.height = 0.17f;
        playerEntity.width = 0.17f;
        playerEntity.isStatic = false;
        playerEntity.gravity_y = -3.0f;
        playerEntity.acceleration_x = 0.0f;
        playerEntity.acceleration_y = 0.0f;
        playerEntity.xFriction = 3.0f;
        playerEntity.entityVector.x = 0.342696;
        playerEntity.entityVector.y = -1.65f;
        playerEntity.entityVector.z = 0.0f;
        playerEntity.waiting = false;
        playerEntity.falling = false;
        playerEntity.returning = false;
        playerEntity.fallen = false;
        vectorOfEnts.push_back(playerEntity);
    }
    
}
void initializeIcicles(std::vector<Entity> &vectorOfEnts, int level)
{
    int index = 570;
    float u = (float)(((int)index) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
    float v = (float)(((int)index) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
    float spriteWidth = 1.0/(float)SPRITE_COUNT_X;
    float spriteHeight = 1.0/(float)SPRITE_COUNT_Y;
    GLuint icicleTexture = LoadTexture("spritesheet.png", false);
    Entity icicle1;
    if(level == 1)
    {
        icicle1.entityType = ENTITY_ICICLE;
        icicle1.EntityTexture = icicleTexture;
        icicle1.xVelocity = 0.0f;
        icicle1.usesSprite = true;
        icicle1.sprite = SheetSprite(icicleTexture, u, v, spriteWidth, spriteHeight, 0.19f);
        icicle1.height = 0.19f;
        icicle1.width = 0.19f;
        icicle1.isStatic = false;
        icicle1.gravity_y = 0.0f;
        icicle1.acceleration_x = 0.0f;
        icicle1.acceleration_y = 0.0f;
        icicle1.xFriction = 0.0f;
        icicle1.entityVector.x = 1.078f;
        icicle1.entityVector.y = -3.76f;
        icicle1.entityVector.z = 0.0f;
        icicle1.waiting = false;
        icicle1.falling = false;
        icicle1.returning = false;
        icicle1.fallen = false;
        vectorOfEnts.push_back(icicle1);
        
        Entity icicle2;
        icicle2.entityType = ENTITY_ICICLE;
        icicle2.EntityTexture = icicleTexture;
        icicle2.xVelocity = 0.0f;
        icicle2.usesSprite = true;
        icicle2.sprite = SheetSprite(icicleTexture, u, v, spriteWidth, spriteHeight, 0.19f);
        icicle2.height = 0.19f;
        icicle2.width = 0.19f;
        icicle2.isStatic = false;
        icicle2.gravity_y = 0.0f;
        icicle2.acceleration_x = 0.0f;
        icicle2.acceleration_y = 0.0f;
        icicle2.xFriction = 0.0f;
        icicle2.entityVector.x = 1.87f;
        icicle2.entityVector.y = -3.75f;
        icicle2.entityVector.z = 0.0f;
        icicle2.waiting = false;
        icicle2.falling = false;
        icicle2.returning = false;
        icicle2.fallen = false;
        vectorOfEnts.push_back(icicle2);
    }
    if(level == 2)
    {
        icicle1.entityType = ENTITY_ICICLE;
        icicle1.EntityTexture = icicleTexture;
        icicle1.xVelocity = 0.0f;
        icicle1.usesSprite = true;
        icicle1.sprite = SheetSprite(icicleTexture, u, v, spriteWidth, spriteHeight, 0.19f);
        icicle1.height = 0.19f;
        icicle1.width = 0.19f;
        icicle1.isStatic = false;
        icicle1.gravity_y = 0.0f;
        icicle1.acceleration_x = 0.0f;
        icicle1.acceleration_y = 0.0f;
        icicle1.xFriction = 0.0f;
        icicle1.entityVector.x = 5.99277f;
        icicle1.entityVector.y = -2.12;
        icicle1.entityVector.z = 0.0f;
        icicle1.waiting = false;
        icicle1.falling = false;
        icicle1.returning = false;
        icicle1.fallen = false;
        vectorOfEnts.push_back(icicle1);
        
        Entity icicle2;
        icicle2.entityType = ENTITY_ICICLE;
        icicle2.EntityTexture = icicleTexture;
        icicle2.xVelocity = 0.0f;
        icicle2.usesSprite = true;
        icicle2.sprite = SheetSprite(icicleTexture, u, v, spriteWidth, spriteHeight, 0.19f);
        icicle2.height = 0.19f;
        icicle2.width = 0.19f;
        icicle2.isStatic = false;
        icicle2.gravity_y = 0.0f;
        icicle2.acceleration_x = 0.0f;
        icicle2.acceleration_y = 0.0f;
        icicle2.xFriction = 0.0f;
        icicle2.entityVector.x = 4.86603f;
        icicle2.entityVector.y = -2.12f;
        icicle2.entityVector.z = 0.0f;
        icicle2.waiting = false;
        icicle2.falling = false;
        icicle2.returning = false;
        icicle2.fallen = false;
        vectorOfEnts.push_back(icicle2);
        
        Entity icicle3;
        icicle3.entityType = ENTITY_ICICLE;
        icicle3.EntityTexture = icicleTexture;
        icicle3.xVelocity = 0.0f;
        icicle3.usesSprite = true;
        icicle3.sprite = SheetSprite(icicleTexture, u, v, spriteWidth, spriteHeight, 0.19f);
        icicle3.height = 0.19f;
        icicle3.width = 0.19f;
        icicle3.isStatic = false;
        icicle3.gravity_y = 0.0f;
        icicle3.acceleration_x = 0.0f;
        icicle3.acceleration_y = 0.0f;
        icicle3.xFriction = 0.0f;
        icicle3.entityVector.x = 3.36557f;
        icicle3.entityVector.y = -2.12f;
        icicle3.entityVector.z = 0.0f;
        icicle3.waiting = false;
        icicle3.falling = false;
        icicle3.returning = false;
        icicle3.fallen = false;
        vectorOfEnts.push_back(icicle3);
        
        Entity icicle4;
        icicle4.entityType = ENTITY_ICICLE;
        icicle4.EntityTexture = icicleTexture;
        icicle4.xVelocity = 0.0f;
        icicle4.usesSprite = true;
        icicle4.sprite = SheetSprite(icicleTexture, u, v, spriteWidth, spriteHeight, 0.19f);
        icicle4.height = 0.19f;
        icicle4.width = 0.19f;
        icicle4.isStatic = false;
        icicle4.gravity_y = 0.0f;
        icicle4.acceleration_x = 0.0f;
        icicle4.acceleration_y = 0.0f;
        icicle4.xFriction = 0.0f;
        icicle4.entityVector.x = 1.78864f;
        icicle4.entityVector.y = -2.12f;
        icicle4.entityVector.z = 0.0f;
        icicle4.waiting = false;
        icicle4.falling = false;
        icicle4.returning = false;
        icicle4.fallen = false;
        vectorOfEnts.push_back(icicle4);
        
        Entity icicle5;
        icicle5.entityType = ENTITY_ICICLE;
        icicle5.EntityTexture = icicleTexture;
        icicle5.xVelocity = 0.0f;
        icicle5.usesSprite = true;
        icicle5.sprite = SheetSprite(icicleTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        icicle5.height = 0.17f;
        icicle5.width = 0.17f;
        icicle5.isStatic = false;
        icicle5.gravity_y = 0.0f;
        icicle5.acceleration_x = 0.0f;
        icicle5.acceleration_y = 0.0f;
        icicle5.xFriction = 0.0f;
        icicle5.entityVector.x = 5.54518f;
        icicle5.entityVector.y = -3.82f;
        icicle5.entityVector.z = 0.0f;
        icicle5.waiting = false;
        icicle5.falling = false;
        icicle5.returning = false;
        icicle5.fallen = false;
        vectorOfEnts.push_back(icicle5);
        
        Entity icicle6;
        icicle6.entityType = ENTITY_ICICLE;
        icicle6.EntityTexture = icicleTexture;
        icicle6.xVelocity = 0.0f;
        icicle6.usesSprite = true;
        icicle6.sprite = SheetSprite(icicleTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        icicle6.height = 0.17f;
        icicle6.width = 0.17f;
        icicle6.isStatic = false;
        icicle6.gravity_y = 0.0f;
        icicle6.acceleration_x = 0.0f;
        icicle6.acceleration_y = 0.0f;
        icicle6.xFriction = 0.0f;
        icicle6.entityVector.x = 6.20f;
        icicle6.entityVector.y = -4.335f;
        icicle6.entityVector.z = 0.0f;
        icicle6.waiting = false;
        icicle6.falling = false;
        icicle6.returning = false;
        icicle6.fallen = false;
        vectorOfEnts.push_back(icicle6);
    }
}

void initializeLollis(std::vector<Entity> &vectorOfEnts, int level)
{
    int index = 626;
    float u = (float)(((int)index) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
    float v = (float)(((int)index) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
    float spriteWidth = 1.0/(float)SPRITE_COUNT_X;
    float spriteHeight = 1.0/(float)SPRITE_COUNT_Y;
    GLuint lolliTexture = LoadTexture("spritesheet.png", false);
    Entity lolli1;
    if(level == 1)
    {
        lolli1.entityType = DEATH_LOLLI;
        lolli1.EntityTexture = lolliTexture;
        lolli1.xVelocity = 0.0f;
        lolli1.usesSprite = true;
        lolli1.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli1.height = 0.17f;
        lolli1.width = 0.17f;
        lolli1.isStatic = false;
        lolli1.gravity_y = 0.0f;
        lolli1.acceleration_x = 0.0f;
        lolli1.acceleration_y = 0.0f;
        lolli1.xFriction = 0.0f;
        lolli1.entityVector.x = 1.8f;
        lolli1.entityVector.y = -1.64209f;
        lolli1.entityVector.z = 0.0f;
        lolli1.waiting = false;
        lolli1.falling = false;
        lolli1.returning = false;
        lolli1.fallen = false;
        vectorOfEnts.push_back(lolli1);
        
        Entity lolli2;
        lolli2.entityType = DEATH_LOLLI;
        lolli2.EntityTexture = lolliTexture;
        lolli2.xVelocity = 0.0f;
        lolli2.usesSprite = true;
        lolli2.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli2.height = 0.17f;
        lolli2.width = 0.17f;
        lolli2.isStatic = false;
        lolli2.gravity_y = 0.0f;
        lolli2.acceleration_x = 0.0f;
        lolli2.acceleration_y = 0.0f;
        lolli2.xFriction = 0.0f;
        lolli2.entityVector.x = 5.34606f;
        lolli2.entityVector.y = -2.14299f;
        lolli2.entityVector.z = 0.0f;
        lolli2.waiting = false;
        lolli2.falling = false;
        lolli2.returning = false;
        lolli2.fallen = false;
        vectorOfEnts.push_back(lolli2);
        
        Entity lolli3;
        lolli3.entityType = DEATH_LOLLI;
        lolli3.EntityTexture = lolliTexture;
        lolli3.xVelocity = 0.0f;
        lolli3.usesSprite = true;
        lolli3.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli3.height = 0.17f;
        lolli3.width = 0.17f;
        lolli3.isStatic = false;
        lolli3.gravity_y = 0.0f;
        lolli3.acceleration_x = 0.0f;
        lolli3.acceleration_y = 0.0f;
        lolli3.xFriction = 0.0f;
        lolli3.entityVector.x = 3.65443f;
        lolli3.entityVector.y = -2.143f;
        lolli3.entityVector.z = 0.0f;
        lolli3.waiting = false;
        lolli3.falling = false;
        lolli3.returning = false;
        lolli3.fallen = false;
        vectorOfEnts.push_back(lolli3);
        
        Entity lolli4;
        lolli4.entityType = DEATH_LOLLI;
        lolli4.EntityTexture = lolliTexture;
        lolli4.xVelocity = 0.0f;
        lolli4.usesSprite = true;
        lolli4.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli4.height = 0.17f;
        lolli4.width = 0.17f;
        lolli4.isStatic = false;
        lolli4.gravity_y = 0.0f;
        lolli4.acceleration_x = 0.0f;
        lolli4.acceleration_y = 0.0f;
        lolli4.xFriction = 0.0f;
        lolli4.entityVector.x = 2.30915f;
        lolli4.entityVector.y = -2.143f;
        lolli4.entityVector.z = 0.0f;
        lolli4.waiting = false;
        lolli4.falling = false;
        lolli4.returning = false;
        lolli4.fallen = false;
        vectorOfEnts.push_back(lolli4);
        
        Entity lolli5;
        lolli5.entityType = DEATH_LOLLI;
        lolli5.EntityTexture = lolliTexture;
        lolli5.xVelocity = 0.0f;
        lolli5.usesSprite = true;
        lolli5.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli5.height = 0.17f;
        lolli5.width = 0.17f;
        lolli5.isStatic = false;
        lolli5.gravity_y = 0.0f;
        lolli5.acceleration_x = 0.0f;
        lolli5.acceleration_y = 0.0f;
        lolli5.xFriction = 0.0f;
        lolli5.entityVector.x = 2.99669f;
        lolli5.entityVector.y = -1.64209f;
        lolli5.entityVector.z = 0.0f;
        lolli5.waiting = false;
        lolli5.falling = false;
        lolli5.returning = false;
        lolli5.fallen = false;
        vectorOfEnts.push_back(lolli5);
        
        Entity lolli6;
        lolli6.entityType = DEATH_LOLLI;
        lolli6.EntityTexture = lolliTexture;
        lolli6.xVelocity = 0.0f;
        lolli6.usesSprite = true;
        lolli6.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli6.height = 0.17f;
        lolli6.width = 0.17f;
        lolli6.isStatic = false;
        lolli6.gravity_y = 0.0f;
        lolli6.acceleration_x = 0.0f;
        lolli6.acceleration_y = 0.0f;
        lolli6.xFriction = 0.0f;
        lolli6.entityVector.x = 4.77594f;
        lolli6.entityVector.y = -1.63632f;
        lolli6.entityVector.z = 0.0f;
        lolli6.waiting = false;
        lolli6.falling = false;
        lolli6.returning = false;
        lolli6.fallen = false;
        vectorOfEnts.push_back(lolli6);
    }
    if(level == 2)
    {
        lolli1.entityType = DEATH_LOLLI;
        lolli1.EntityTexture = lolliTexture;
        lolli1.xVelocity = 0.0f;
        lolli1.usesSprite = true;
        lolli1.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli1.height = 0.17f;
        lolli1.width = 0.17f;
        lolli1.isStatic = false;
        lolli1.gravity_y = 0.0f;
        lolli1.acceleration_x = 0.0f;
        lolli1.acceleration_y = 0.0f;
        lolli1.xFriction = 0.0f;
        lolli1.entityVector.x = 1.32466f;
        lolli1.entityVector.y = -3.4851f;
        lolli1.entityVector.z = 0.0f;
        lolli1.waiting = false;
        lolli1.falling = false;
        lolli1.returning = false;
        lolli1.fallen = false;
        vectorOfEnts.push_back(lolli1);
        
        Entity lolli2;
        lolli2.entityType = DEATH_LOLLI;
        lolli2.EntityTexture = lolliTexture;
        lolli2.xVelocity = 0.0f;
        lolli2.usesSprite = true;
        lolli2.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli2.height = 0.17f;
        lolli2.width = 0.17f;
        lolli2.isStatic = false;
        lolli2.gravity_y = 0.0f;
        lolli2.acceleration_x = 0.0f;
        lolli2.acceleration_y = 0.0f;
        lolli2.xFriction = 0.0f;
        lolli2.entityVector.x = 1.32466f;
        lolli2.entityVector.y = -3.30f;
        lolli2.entityVector.z = 0.0f;
        lolli2.waiting = false;
        lolli2.falling = false;
        lolli2.returning = false;
        lolli2.fallen = false;
        vectorOfEnts.push_back(lolli2);
        
        Entity lolli5;
        lolli5.entityType = DEATH_LOLLI;
        lolli5.EntityTexture = lolliTexture;
        lolli5.xVelocity = 0.0f;
        lolli5.usesSprite = true;
        lolli5.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli5.height = 0.17f;
        lolli5.width = 0.17f;
        lolli5.isStatic = false;
        lolli5.gravity_y = 0.0f;
        lolli5.acceleration_x = 0.0f;
        lolli5.acceleration_y = 0.0f;
        lolli5.xFriction = 0.0f;
        lolli5.entityVector.x = 2.35035f;
        lolli5.entityVector.y = -3.4851f;
        lolli5.entityVector.z = 0.0f;
        lolli5.waiting = false;
        lolli5.falling = false;
        lolli5.returning = false;
        lolli5.fallen = false;
        vectorOfEnts.push_back(lolli5);
        
        Entity lolli6;
        lolli6.entityType = DEATH_LOLLI;
        lolli6.EntityTexture = lolliTexture;
        lolli6.xVelocity = 0.0f;
        lolli6.usesSprite = true;
        lolli6.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli6.height = 0.17f;
        lolli6.width = 0.17f;
        lolli6.isStatic = false;
        lolli6.gravity_y = 0.0f;
        lolli6.acceleration_x = 0.0f;
        lolli6.acceleration_y = 0.0f;
        lolli6.xFriction = 0.0f;
        lolli6.entityVector.x = 2.35035f;
        lolli6.entityVector.y = -3.30f;
        lolli6.entityVector.z = 0.0f;
        lolli6.waiting = false;
        lolli6.falling = false;
        lolli6.returning = false;
        lolli6.fallen = false;
        vectorOfEnts.push_back(lolli6);
        
        Entity lolli7;
        lolli7.entityType = DEATH_LOLLI;
        lolli7.EntityTexture = lolliTexture;
        lolli7.xVelocity = 0.0f;
        lolli7.usesSprite = true;
        lolli7.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli7.height = 0.17f;
        lolli7.width = 0.17f;
        lolli7.isStatic = false;
        lolli7.gravity_y = 0.0f;
        lolli7.acceleration_x = 0.0f;
        lolli7.acceleration_y = 0.0f;
        lolli7.xFriction = 0.0f;
        lolli7.entityVector.x = 3.30124f;
        lolli7.entityVector.y = -3.4851f;
        lolli7.entityVector.z = 0.0f;
        lolli7.waiting = false;
        lolli7.falling = false;
        lolli7.returning = false;
        lolli7.fallen = false;
        vectorOfEnts.push_back(lolli7);
        
        Entity lolli8;
        lolli8.entityType = DEATH_LOLLI;
        lolli8.EntityTexture = lolliTexture;
        lolli8.xVelocity = 0.0f;
        lolli8.usesSprite = true;
        lolli8.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli8.height = 0.17f;
        lolli8.width = 0.17f;
        lolli8.isStatic = false;
        lolli8.gravity_y = 0.0f;
        lolli8.acceleration_x = 0.0f;
        lolli8.acceleration_y = 0.0f;
        lolli8.xFriction = 0.0f;
        lolli8.entityVector.x = 3.30124f;
        lolli8.entityVector.y = -3.30f;
        lolli8.entityVector.z = 0.0f;
        lolli8.waiting = false;
        lolli8.falling = false;
        lolli8.returning = false;
        lolli8.fallen = false;
        vectorOfEnts.push_back(lolli8);
        
        /*
        Entity lolli9;
        lolli9.entityType = DEATH_LOLLI;
        lolli9.EntityTexture = lolliTexture;
        lolli9.xVelocity = 0.0f;
        lolli9.usesSprite = true;
        lolli9.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli9.height = 0.17f;
        lolli9.width = 0.17f;
        lolli9.isStatic = false;
        lolli9.gravity_y = 0.0f;
        lolli9.acceleration_x = 0.0f;
        lolli9.acceleration_y = 0.0f;
        lolli9.xFriction = 0.0f;
        lolli9.entityVector.x = 3.91158f;
        lolli9.entityVector.y = -3.4851f;
        lolli9.entityVector.z = 0.0f;
        lolli9.waiting = false;
        lolli9.falling = false;
        lolli9.returning = false;
        lolli9.fallen = false;
        vectorOfEnts.push_back(lolli9);
        
        Entity lolli10;
        lolli10.entityType = DEATH_LOLLI;
        lolli10.EntityTexture = lolliTexture;
        lolli10.xVelocity = 0.0f;
        lolli10.usesSprite = true;
        lolli10.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli10.height = 0.17f;
        lolli10.width = 0.17f;
        lolli10.isStatic = false;
        lolli10.gravity_y = 0.0f;
        lolli10.acceleration_x = 0.0f;
        lolli10.acceleration_y = 0.0f;
        lolli10.xFriction = 0.0f;
        lolli10.entityVector.x = 3.91158f;
        lolli10.entityVector.y = -3.30f;
        lolli10.entityVector.z = 0.0f;
        lolli10.waiting = false;
        lolli10.falling = false;
        lolli10.returning = false;
        lolli10.fallen = false;
        vectorOfEnts.push_back(lolli10);
        */
        Entity lolli11;
        lolli11.entityType = DEATH_LOLLI;
        lolli11.EntityTexture = lolliTexture;
        lolli11.xVelocity = 0.0f;
        lolli11.usesSprite = true;
        lolli11.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli11.height = 0.17f;
        lolli11.width = 0.17f;
        lolli11.isStatic = false;
        lolli11.gravity_y = 0.0f;
        lolli11.acceleration_x = 0.0f;
        lolli11.acceleration_y = 0.0f;
        lolli11.xFriction = 0.0f;
        lolli11.entityVector.x = 4.35486f;
        lolli11.entityVector.y = -3.4851f;
        lolli11.entityVector.z = 0.0f;
        lolli11.waiting = false;
        lolli11.falling = false;
        lolli11.returning = false;
        lolli11.fallen = false;
        vectorOfEnts.push_back(lolli11);
        
        Entity lolli12;
        lolli12.entityType = DEATH_LOLLI;
        lolli12.EntityTexture = lolliTexture;
        lolli12.xVelocity = 0.0f;
        lolli12.usesSprite = true;
        lolli12.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli12.height = 0.17f;
        lolli12.width = 0.17f;
        lolli12.isStatic = false;
        lolli12.gravity_y = 0.0f;
        lolli12.acceleration_x = 0.0f;
        lolli12.acceleration_y = 0.0f;
        lolli12.xFriction = 0.0f;
        lolli12.entityVector.x = 4.35486f;
        lolli12.entityVector.y = -3.30f;
        lolli12.entityVector.z = 0.0f;
        lolli12.waiting = false;
        lolli12.falling = false;
        lolli12.returning = false;
        lolli12.fallen = false;
        vectorOfEnts.push_back(lolli12);
        
        Entity lolli13;
        lolli13.entityType = DEATH_LOLLI;
        lolli13.EntityTexture = lolliTexture;
        lolli13.xVelocity = 0.0f;
        lolli13.usesSprite = true;
        lolli13.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli13.height = 0.17f;
        lolli13.width = 0.17f;
        lolli13.isStatic = false;
        lolli13.gravity_y = 0.0f;
        lolli13.acceleration_x = 0.0f;
        lolli13.acceleration_y = 0.0f;
        lolli13.xFriction = 0.0f;
        lolli13.entityVector.x = 5.30f;
        lolli13.entityVector.y = -3.4851f;
        lolli13.entityVector.z = 0.0f;
        lolli13.waiting = false;
        lolli13.falling = false;
        lolli13.returning = false;
        lolli13.fallen = false;
        vectorOfEnts.push_back(lolli13);
        
        Entity lolli14;
        lolli14.entityType = DEATH_LOLLI;
        lolli14.EntityTexture = lolliTexture;
        lolli14.xVelocity = 0.0f;
        lolli14.usesSprite = true;
        lolli14.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli14.height = 0.17f;
        lolli14.width = 0.17f;
        lolli14.isStatic = false;
        lolli14.gravity_y = 0.0f;
        lolli14.acceleration_x = 0.0f;
        lolli14.acceleration_y = 0.0f;
        lolli14.xFriction = 0.0f;
        lolli14.entityVector.x = 5.30f;
        lolli14.entityVector.y = -3.30f;
        lolli14.entityVector.z = 0.0f;
        lolli14.waiting = false;
        lolli14.falling = false;
        lolli14.returning = false;
        lolli14.fallen = false;
        vectorOfEnts.push_back(lolli14);
        
        Entity lolli15;
        lolli15.entityType = DEATH_LOLLI;
        lolli15.EntityTexture = lolliTexture;
        lolli15.xVelocity = 0.0f;
        lolli15.usesSprite = true;
        lolli15.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli15.height = 0.17f;
        lolli15.width = 0.17f;
        lolli15.isStatic = false;
        lolli15.gravity_y = 0.0f;
        lolli15.acceleration_x = 0.0f;
        lolli15.acceleration_y = 0.0f;
        lolli15.xFriction = 0.0f;
        lolli15.entityVector.x = 6.31716f;
        lolli15.entityVector.y = -3.4851f;
        lolli15.entityVector.z = 0.0f;
        lolli15.waiting = false;
        lolli15.falling = false;
        lolli15.returning = false;
        lolli15.fallen = false;
        vectorOfEnts.push_back(lolli15);
        
        Entity lolli16;
        lolli16.entityType = DEATH_LOLLI;
        lolli16.EntityTexture = lolliTexture;
        lolli16.xVelocity = 0.0f;
        lolli16.usesSprite = true;
        lolli16.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli16.height = 0.17f;
        lolli16.width = 0.17f;
        lolli16.isStatic = false;
        lolli16.gravity_y = 0.0f;
        lolli16.acceleration_x = 0.0f;
        lolli16.acceleration_y = 0.0f;
        lolli16.xFriction = 0.0f;
        lolli16.entityVector.x = 6.31716f;
        lolli16.entityVector.y = -3.30f;
        lolli16.entityVector.z = 0.0f;
        lolli16.waiting = false;
        lolli16.falling = false;
        lolli16.returning = false;
        lolli16.fallen = false;
        vectorOfEnts.push_back(lolli16);
        
        Entity lolli17;
        lolli17.entityType = DEATH_LOLLI;
        lolli17.EntityTexture = lolliTexture;
        lolli17.xVelocity = 0.0f;
        lolli17.usesSprite = true;
        lolli17.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli17.height = 0.17f;
        lolli17.width = 0.17f;
        lolli17.isStatic = false;
        lolli17.gravity_y = 0.0f;
        lolli17.acceleration_x = 0.0f;
        lolli17.acceleration_y = 0.0f;
        lolli17.xFriction = 0.0f;
        lolli17.entityVector.x = 1.32466f;
        lolli17.entityVector.y = -3.4851f;
        lolli17.entityVector.z = 0.0f;
        lolli17.waiting = false;
        lolli17.falling = false;
        lolli17.returning = false;
        lolli17.fallen = false;
        vectorOfEnts.push_back(lolli17);
        
        Entity lolli18;
        lolli18.entityType = DEATH_LOLLI;
        lolli18.EntityTexture = lolliTexture;
        lolli18.xVelocity = 0.0f;
        lolli18.usesSprite = true;
        lolli18.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli18.height = 0.17f;
        lolli18.width = 0.17f;
        lolli18.isStatic = false;
        lolli18.gravity_y = 0.0f;
        lolli18.acceleration_x = 0.0f;
        lolli18.acceleration_y = 0.0f;
        lolli18.xFriction = 0.0f;
        lolli18.entityVector.x = 1.32466f;
        lolli18.entityVector.y = -3.30f;
        lolli18.entityVector.z = 0.0f;
        lolli18.waiting = false;
        lolli18.falling = false;
        lolli18.returning = false;
        lolli18.fallen = false;
        vectorOfEnts.push_back(lolli18);
        
        Entity lolli19;
        lolli19.entityType = DEATH_LOLLI;
        lolli19.EntityTexture = lolliTexture;
        lolli19.xVelocity = 0.0f;
        lolli19.usesSprite = true;
        lolli19.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli19.height = 0.17f;
        lolli19.width = 0.17f;
        lolli19.isStatic = false;
        lolli19.gravity_y = 0.0f;
        lolli19.acceleration_x = 0.0f;
        lolli19.acceleration_y = 0.0f;
        lolli19.xFriction = 0.0f;
        lolli19.entityVector.x = 0.60f;
        lolli19.entityVector.y = -4.17f;
        lolli19.entityVector.z = 0.0f;
        lolli19.waiting = false;
        lolli19.falling = false;
        lolli19.returning = false;
        lolli19.fallen = false;
        vectorOfEnts.push_back(lolli19);
        
        Entity lolli20;
        lolli20.entityType = DEATH_LOLLI;
        lolli20.EntityTexture = lolliTexture;
        lolli20.xVelocity = 0.0f;
        lolli20.usesSprite = true;
        lolli20.sprite = SheetSprite(lolliTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        lolli20.height = 0.17f;
        lolli20.width = 0.17f;
        lolli20.isStatic = false;
        lolli20.gravity_y = 0.0f;
        lolli20.acceleration_x = 0.0f;
        lolli20.acceleration_y = 0.0f;
        lolli20.xFriction = 0.0f;
        lolli20.entityVector.x = 0.255f;
        lolli20.entityVector.y = -5.0151f;
        lolli20.entityVector.z = 0.0f;
        lolli20.waiting = false;
        lolli20.falling = false;
        lolli20.returning = false;
        lolli20.fallen = false;
        vectorOfEnts.push_back(lolli20);
    }
}
void initializeTurrets(std::vector<Entity> &vectorOfEnts, int level)
{
    int index = 14;
    float u = (float)(((int)index) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
    float v = (float)(((int)index) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
    float spriteWidth = 1.0/(float)SPRITE_COUNT_X;
    float spriteHeight = 1.0/(float)SPRITE_COUNT_Y;
    GLuint turretTexture = LoadTexture("spritesheet.png", false);
    Entity turret1;
    if(level == 2)
    {
        turret1.entityType = TURRET;
        turret1.EntityTexture = turretTexture;
        turret1.xVelocity = 0.0f;
        turret1.usesSprite = true;
        turret1.sprite = SheetSprite(turretTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        turret1.height = 0.17f;
        turret1.width = 0.17f;
        turret1.isStatic = false;
        turret1.gravity_y = 0.0f;
        turret1.acceleration_x = 0.0f;
        turret1.acceleration_y = 0.0f;
        turret1.xFriction = 0.0f;
        turret1.entityVector.x = 5.77775f;
        turret1.entityVector.y = -1.7851f;
        turret1.entityVector.z = 0.0f;
        turret1.rotation = -1.0f;
        turret1.waiting = false;
        turret1.falling = false;
        turret1.returning = false;
        turret1.fallen = false;
        vectorOfEnts.push_back(turret1);
        
        Entity turret2;
        turret2.entityType = TURRET;
        turret2.EntityTexture = turretTexture;
        turret2.xVelocity = 0.0f;
        turret2.usesSprite = true;
        turret2.sprite = SheetSprite(turretTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        turret2.height = 0.17f;
        turret2.width = 0.17f;
        turret2.isStatic = false;
        turret2.gravity_y = 0.0f;
        turret2.acceleration_x = 0.0f;
        turret2.acceleration_y = 0.0f;
        turret2.xFriction = 0.0f;
        turret2.entityVector.x = 0.778831f;
        turret2.entityVector.y = -2.6351f;
        turret2.entityVector.z = 0.0f;
        turret2.rotation = 1.0f;
        turret2.waiting = false;
        turret2.falling = false;
        turret2.returning = false;
        turret2.fallen = false;
        vectorOfEnts.push_back(turret2);
        
        Entity turret3;
        turret3.entityType = TURRET;
        turret3.EntityTexture = turretTexture;
        turret3.xVelocity = 0.0f;
        turret3.usesSprite = true;
        turret3.sprite = SheetSprite(turretTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        turret3.height = 0.17f;
        turret3.width = 0.17f;
        turret3.isStatic = false;
        turret3.gravity_y = 0.0f;
        turret3.acceleration_x = 0.0f;
        turret3.acceleration_y = 0.0f;
        turret3.xFriction = 0.0f;
        turret3.entityVector.x = 0.80f;
        turret3.entityVector.y = -4.0f;
        turret3.entityVector.z = 0.0f;
        turret3.rotation = 1.0f;
        turret3.waiting = false;
        turret3.falling = false;
        turret3.returning = false;
        turret3.fallen = false;
        vectorOfEnts.push_back(turret3);
    }

}
void initializeFists(std::vector<Entity> &vectorOfEnts, int level)
{
    int index = 870;
    float u = (float)(((int)index) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
    float v = (float)(((int)index) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
    float spriteWidth = 1.0/(float)SPRITE_COUNT_X;
    float spriteHeight = 1.0/(float)SPRITE_COUNT_Y;
    GLuint fistTexture = LoadTexture("spritesheet.png", false);
    if(level == 1)
    {
        Entity fist1;
        fist1.entityType = ENTITY_FIST;
        fist1.EntityTexture = fistTexture;
        fist1.xVelocity = 0.0f;
        fist1.usesSprite = true;
        fist1.sprite = SheetSprite(fistTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        fist1.height = 0.17f;
        fist1.width = 0.17f;
        fist1.isStatic = false;
        fist1.gravity_y = 0.0f;
        fist1.acceleration_x = 0.0f;
        fist1.acceleration_y = 0.0f;
        fist1.xFriction = 0.0f;
        fist1.entityVector.x = 2.05f;
        fist1.startingPosition = -2.97f;
        fist1.entityVector.y = -2.97f;
        fist1.entityVector.z = 0.0f;
        fist1.waiting = true;
        vectorOfEnts.push_back(fist1);
        
        Entity fist2;
        fist2.entityType = ENTITY_FIST;
        fist2.EntityTexture = fistTexture;
        fist2.xVelocity = 0.0f;
        fist2.usesSprite = true;
        fist2.sprite = SheetSprite(fistTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        fist2.height = 0.17f;
        fist2.width = 0.17f;
        fist2.isStatic = false;
        fist2.gravity_y = 0.0f;
        fist2.acceleration_x = 0.0f;
        fist2.acceleration_y = 0.0f;
        fist2.xFriction = 0.0f;
        fist2.startingPosition = -2.45f;
        fist2.entityVector.x = 4.36f;
        fist2.entityVector.y = -2.45f;
        fist2.entityVector.z = 0.0f;
        fist2.waiting = true;
        vectorOfEnts.push_back(fist2);
    }
}
void initializePlatform(std::vector<Entity> &vectorOfEnts, int level)
{
    int index = 43;
    float u = (float)(((int)index) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
    float v = (float)(((int)index) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
    float spriteWidth = 1.0/(float)SPRITE_COUNT_X;
    float spriteHeight = 1.0/(float)SPRITE_COUNT_Y;
    GLuint platformTexture = LoadTexture("spritesheet.png", false);
    if(level == 1)
    {
        Entity platform1;
        platform1.entityType = ENTITY_PLATFORM;
        platform1.EntityTexture = platformTexture;
        platform1.xVelocity = -1.2f;
        platform1.usesSprite = true;
        platform1.sprite = SheetSprite(platformTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        platform1.height = 0.17f;
        platform1.width = 0.17f;
        platform1.isStatic = false;
        platform1.gravity_y = 0.0f;
        platform1.acceleration_x = 0.0f;
        platform1.acceleration_y = 0.0f;
        platform1.xFriction = 0.0f;
        platform1.entityVector.x = 6.26589f;
        platform1.startingPosition = 6.26589f;
        platform1.entityVector.y = -2.20f;
        platform1.entityVector.z = 0.0f;
        platform1.waiting = false;
        platform1.falling = true;
        platform1.returning = false;
        platform1.fallen = false;
        vectorOfEnts.push_back(platform1);
        
        Entity platform2;
        platform2.entityType = ENTITY_PLATFORM;
        platform2.EntityTexture = platformTexture;
        platform2.xVelocity = -1.2f;
        platform2.usesSprite = true;
        platform2.sprite = SheetSprite(platformTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        platform2.height = 0.17f;
        platform2.width = 0.17f;
        platform2.isStatic = false;
        platform2.gravity_y = 0.0f;
        platform2.acceleration_x = 0.0f;
        platform2.acceleration_y = 0.0f;
        platform2.xFriction = 0.0f;
        platform2.entityVector.x = 6.10589f;
        platform2.startingPosition = 6.10589f;
        platform2.entityVector.y = -2.20f;
        platform2.entityVector.z = 0.0f;
        platform2.waiting = false;
        platform2.falling = true;
        platform2.returning = false;
        platform2.fallen = false;
        vectorOfEnts.push_back(platform2);
        
        Entity platform3;
        platform3.entityType = ENTITY_PLATFORM;
        platform3.EntityTexture = platformTexture;
        platform3.xVelocity = -1.2f;
        platform3.usesSprite = true;
        platform3.sprite = SheetSprite(platformTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        platform3.height = 0.17f;
        platform3.width = 0.17f;
        platform3.isStatic = false;
        platform3.gravity_y = 0.0f;
        platform3.acceleration_x = 0.0f;
        platform3.acceleration_y = 0.0f;
        platform3.xFriction = 0.0f;
        platform3.entityVector.x = 6.43f;
        platform3.startingPosition = 6.43f;
        platform3.entityVector.y = -2.20f;
        platform3.entityVector.z = 0.0f;
        platform3.waiting = false;
        platform3.falling = true;
        platform3.returning = false;
        platform3.fallen = false;
        vectorOfEnts.push_back(platform3);
    }
}
void initializeEndOfLevel(std::vector<Entity> &vectorOfEnts, int level)
{
    int index = 311;
    float u = (float)(((int)index) % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
    float v = (float)(((int)index) / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;
    float spriteWidth = 1.0/(float)SPRITE_COUNT_X;
    float spriteHeight = 1.0/(float)SPRITE_COUNT_Y;
    GLuint flagTexture = LoadTexture("spritesheet.png", false);
    if(level == 1)
    {
        Entity flagEntity;
        flagEntity.entityType = LEVEL_GOAL;
        flagEntity.EntityTexture = flagTexture;
        flagEntity.xVelocity = 0;
        flagEntity.usesSprite = true;
        flagEntity.sprite = SheetSprite(flagTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        flagEntity.height = 0.17f;
        flagEntity.width = 0.17f;
        flagEntity.isStatic = false;
        flagEntity.gravity_y = 0.0f;
        flagEntity.acceleration_x = 0.0f;
        flagEntity.acceleration_y = 0.0f;
        flagEntity.xFriction = 3.0f;
        flagEntity.entityVector.x = 0.426;
        flagEntity.entityVector.y = -1.955f;
        flagEntity.entityVector.z = 0.0f;
        flagEntity.waiting = false;
        flagEntity.falling = false;
        flagEntity.returning = false;
        flagEntity.fallen = false;
        vectorOfEnts.push_back(flagEntity);
    }
    if(level == 2)
    {
        Entity flagEntity;
        flagEntity.entityType = LEVEL_GOAL;
        flagEntity.EntityTexture = flagTexture;
        flagEntity.xVelocity = 0;
        flagEntity.usesSprite = true;
        flagEntity.sprite = SheetSprite(flagTexture, u, v, spriteWidth, spriteHeight, 0.17f);
        flagEntity.height = 0.17f;
        flagEntity.width = 0.17f;
        flagEntity.isStatic = false;
        flagEntity.gravity_y = 0.0f;
        flagEntity.acceleration_x = 0.0f;
        flagEntity.acceleration_y = 0.0f;
        flagEntity.xFriction = 3.0f;
        flagEntity.entityVector.x = 1.6669f;
        flagEntity.entityVector.y = -5.0151f;
        flagEntity.entityVector.z = 0.0f;
        flagEntity.waiting = false;
        flagEntity.falling = false;
        flagEntity.returning = false;
        flagEntity.fallen = false;
        vectorOfEnts.push_back(flagEntity);
    }
}
void resetGame(int levelNumber, std::vector<Entity> &vectorOfEnts)
{
    if(levelNumber == 1)
    {
        initializeEntitiesLevel1(vectorOfEnts);
    }
    if(levelNumber == 2)
    {
        initializeEntitiesLevel2(vectorOfEnts);
    }
}