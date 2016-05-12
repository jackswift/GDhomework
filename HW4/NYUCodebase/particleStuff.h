//
//  particleStuff.h
//  NYUCodebase
//
//  Created by Lyle True on 5/12/16.
//  Copyright (c) 2016 Ivan Safrin. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "Vector.h"
#include <vector>
#include "ShaderProgram.h"

class Particle
{
public:
    Vector position;
    Vector velocity;
    float lifetime;
};

class ParticleEmitter {
public:
    
    ParticleEmitter(unsigned int particleCount);
    ParticleEmitter();
    ~ParticleEmitter();
    void Update(float elapsed);
    void Render(ShaderProgram program);
    Vector position;
    Vector gravity;
    float maxLifetime;
    std::vector<Particle> particles;
};