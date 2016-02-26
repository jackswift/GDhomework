//
//  LoadAndDrawFuncs.h
//  NYUCodebase
//
//  Created by Lyle True on 2/25/16.
//  Copyright (c) 2016 Ivan Safrin. All rights reserved.
//

#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include <vector>



void DrawSpriteSheet(ShaderProgram &program, GLuint theTexture, int index, int spriteCountX, int spriteCountY);
void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing);
GLuint LoadTexture(const char *image_path, bool isRepeating);
void drawTexture(GLuint theTexture, ShaderProgram program, float height, float width);