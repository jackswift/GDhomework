//
//  Vector.cpp
//  NYUCodebase
//
//  Created by Lyle True on 4/7/16.
//  Copyright (c) 2016 Ivan Safrin. All rights reserved.
//

#include "Vector.h"



Vector Vector::operator*(const Vector& Vec)
{
    Vector vecToReturn;
    
    vecToReturn.x = this->x * Vec.x;
    vecToReturn.y = this->y * Vec.y;
    vecToReturn.z = this->z * Vec.z;
    
    return vecToReturn;
}