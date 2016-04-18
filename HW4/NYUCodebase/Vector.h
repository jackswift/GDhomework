//
//  Vector.h
//  NYUCodebase
//
//  Created by Lyle True on 4/7/16.
//  Copyright (c) 2016 Ivan Safrin. All rights reserved.
//


#pragma once

class Vector
{
public:
    float x;
    float y;
    float z;
    
    Vector operator*(const Vector&);
    
private:
};
