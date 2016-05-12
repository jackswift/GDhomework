//
//  particleStuff.cpp
//  NYUCodebase
//
//  Created by Lyle True on 5/12/16.
//  Copyright (c) 2016 Ivan Safrin. All rights reserved.
//

#include "particleStuff.h"



void ParticleEmitter::Render(ShaderProgram program)
{
    std::vector<float> vertices;
    for(int i=0; i < particles.size(); i++) {
        vertices.push_back(particles[i].position.x);
        vertices.push_back(particles[i].position.y);
    }
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_POINTS, 0, vertices.size()/2);
}