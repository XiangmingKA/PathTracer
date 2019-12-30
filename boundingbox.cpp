//
//  boundingbox.cpp
//  CS285_Assignment3_1.0
//
//  Created by 喀相明 on 2019/5/18.
//  Copyright © 2019 喀相明. All rights reserved.
//

#include "boundingbox.h"
#include "csphere.h"
#include "cray.h"



bool aabb::hit (CRay& Ray, float tmin, float tmax){
    float rayDir[3];
    rayDir[0] = Ray.getDirection().x;
    rayDir[1] = Ray.getDirection().y;
    rayDir[2] = Ray.getDirection().z;
    float rayOri[3];
    rayOri[0] = Ray.getOrigin().x;
    rayOri[1] = Ray.getOrigin().y;
    rayOri[2] = Ray.getOrigin().z;

    for(int i =0; i <3; i++){
        //0,0 <>
        float invD = 1.0f / rayDir[i];
        float t0 = (min[i] - rayOri[i])*invD;
        float t1 = (max[i] - rayOri[i])*invD;
        if(invD < 0.0f)
            std::swap(t0, t1);
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if(tmax <= tmin)
            return false;
    }

    return true;
}

