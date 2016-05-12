//
//  initializiationStuff.h
//  NYUCodebase
//
//  Created by Lyle True on 5/9/16.
//  Copyright (c) 2016 Ivan Safrin. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Entity.h"
#include "LoadAndDrawFuncs.h"


#define SPRITE_COUNT_X 30
#define SPRITE_COUNT_Y 30

void initializeEntitiesLevel1(std::vector<Entity> &vectorOfEnts);
void initializePlayer(std::vector<Entity> &vectorOfEnts, int level);
void initializeIcicles(std::vector<Entity> &vectorOfEnts, int level);
void initializeFists(std::vector<Entity> &vectorOfEnts, int level);
void initializePlatform(std::vector<Entity> &vectorOfEnts, int level);
void resetGame(int levelNumber, std::vector<Entity> &vectorOfEnts);
void initializeEndOfLevel(std::vector<Entity> &vectorOfEnts, int level);
void initializeEntitiesLevel2(std::vector<Entity> &vectorOfEnts);
void initializeLollis(std::vector<Entity> &vectorOfEnts, int level);
void initializeTurrets(std::vector<Entity> &vectorOfEnts, int level);