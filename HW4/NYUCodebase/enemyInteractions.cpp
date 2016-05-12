//
//  enemyInteractions.cpp
//  NYUCodebase
//
//  Created by Lyle True on 5/9/16.
//  Copyright (c) 2016 Ivan Safrin. All rights reserved.
//

#include "enemyInteractions.h"
#include "initializiationStuff.h"

void icicleFalling(Entity &icicleToFall)
{
    icicleToFall.yVelocity = -1.8f;
}

void playerDies(float &timeDead, float elapsed, int currentLevel)
{
    timeDead+= elapsed;
    if(timeDead >= 3.0f)
    {
        //resetGame(currentLevel, );
    }
}
void fistFalling(Entity &fistToFall)
{
    fistToFall.falling = true;
    fistToFall.fallen = false;
    fistToFall.waiting = false;
    fistToFall.returning = false;
    //std::cout << "I AM FALLING NOW!" << std::endl;
    fistToFall.yVelocity = -1.2f;
}

void fistReturning(Entity &fistToReturn)
{
    fistToReturn.falling = false;
    fistToReturn.fallen = false;
    fistToReturn.waiting = false;
    fistToReturn.returning = true;
    fistToReturn.yVelocity = 0.8f;
    fistToReturn.timeFallen = 0.0f;
    //std::cout << "I shall return!" << std::endl;
}
void pushPlayerToTopOfFist(std::vector<Entity> &vectorOfEnts, int indexOfFist)
{
    vectorOfEnts[0].entityVector.y = vectorOfEnts[indexOfFist].entityVector.y + vectorOfEnts[indexOfFist].height +0.000001f;
}