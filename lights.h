//
//  lights.hpp
//  CS285_Assignment2_1.0
//
//  Created by 喀相明 on 2019/5/12.
//  Copyright © 2019 喀相明. All rights reserved.
//

#ifndef lights_h
#define lights_h

#include <stdio.h>
#include "cobject.h"
#include "gvector3.h"
#include "material.h"

struct IntersectResult;
class Union;

class Light
{
public:
    Light();
    virtual~Light();
    virtual GColor intersect(Union &scence,IntersectResult &result);
    virtual GVector3 getLightDir(GVector3 pos);
};

class lightUnion:public Light
{
public:
    lightUnion();
    virtual ~lightUnion();
    void push(Light* light);
    virtual GColor intersect(Union &scence,IntersectResult &result);

    vector<Light*> lights;
};


class DirectLight:public Light
{
public:
    DirectLight();
    DirectLight(GColor _color,GVector3 _direction,bool _isShadow);
    virtual ~DirectLight();
    virtual GColor intersect(Union &scence,IntersectResult &result);
    virtual GVector3 getLightDir(GVector3 pos);
protected:
private:
    bool isShadow;
    GColor color;
    GVector3 direction;
};

class PointLight:public Light
{
public:
    PointLight();
    PointLight(GColor _color,GVector3 _position,bool _isShadow);
    virtual ~PointLight();
    virtual GColor intersect(Union &scence,IntersectResult &result);
    virtual GVector3 getLightDir(GVector3 pos);
protected:
private:
    bool isShadow;
    GColor color;
    GVector3 position;
};


#endif /* lights_h */
