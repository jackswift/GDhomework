//
//  enemyInteractions.h
//  NYUCodebase
//
//  Created by Lyle True on 5/9/16.
//  Copyright (c) 2016 Ivan Safrin. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "Entity.h"
#include <vector>


void icicleFalling(Entity &icicleToFall);
void fistFalling(Entity &fistToFall);
void fistReturning(Entity &fistToReturn);
void pushPlayerToTopOfFist(std::vector<Entity> &vectorOfEnts, int indexOfFist);
void playerDies(float &timeDead, float elapsed, int currentLevel);

