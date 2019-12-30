//
//  boundingbox.hpp
//  CS285_Assignment3_1.0
//
//  Created by 喀相明 on 2019/5/18.
//  Copyright © 2019 喀相明. All rights reserved.
//

#ifndef boundingbox_hpp
#define boundingbox_hpp

#include <stdio.h>
#include "cobject.h"
#include "cray.h"
#include "gvector3.h"

class CObject;

class aabb{

public:
    aabb(){}
    aabb(float _min[], float _max[],CObject* obj){
        for(int i = 0; i <3 ; i++){
            min[i] = _min[i];
            max[i] = _max[i];
            object = obj;
        }
    }

    GVector3 getMin(){
        return GVector3(min[0],min[1],min[2]);
    }
    GVector3 getMax(){
        return GVector3(max[0],max[1],max[2]);
    }
    float min[3];
    float max[3];
    CObject* object;
    //vector<CObject*> cobjects;
    bool hit(CRay& Ray, float tmin, float tmax);
};




#endif /* boundingbox_hpp */
