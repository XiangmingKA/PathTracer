//
//  material.hpp
//  CS285_Assignment2_1.0
//
//  Created by 喀相明 on 2019/5/12.
//  Copyright © 2019 喀相明. All rights reserved.
//

#ifndef material_hpp
#define material_hpp

#include "gvector3.h"
#include "cray.h"
#include "lights.h"

class IntersectResult;

using namespace std;

class Light;


class Material
{
public:
    Material();
    Material(float _reflectiveness);
    float getRef();
    void setRef(float _reflectiveness);
    virtual ~Material();
    //virtual GColor sample(const CRay& ray,const GVector3& position,const GVector3& normal);
protected:
    float reflectiveness = 0;
private:

};

static GVector3 lightDir = GVector3(1, 1, 1).normalize();
static GColor lightColor = GColor::white();
class PhongMaterial:public Material
{
public:
    PhongMaterial();
    PhongMaterial(const GColor& _diffuse,const GColor& _specular,const float& _shininess,float _reflectiveness=0);
    PhongMaterial(const GColor& _ambient, const GColor& _diffuse,const GColor& _specular,const float& _shininess,float _reflectiveness, float Ktran);
    virtual GColor sample(CRay& ray,GVector3& position,GVector3& normal, IntersectResult* result);
    virtual GColor sample(CRay& ray,GVector3& position,GVector3& normal);
    virtual GColor sample(CRay& ray,GVector3& position,GVector3& normal, Light light);
    virtual GColor sample(CRay& ray,GVector3& position,GVector3& normal, float u, float v);
    virtual GColor sampleOutter(CRay& ray,GVector3& position,GVector3& normal,IntersectResult* tempRes );
    virtual ~PhongMaterial();
    GColor   ambient = GColor(0,0,0);
    GColor   diffuse = GColor(0,0,0);
    GColor   specular = GColor(0,0,0);
    GColor v1Color;
    GColor v2Color;
    GColor v3Color;
    bool usingInterp = false;
    float    shininess;
    float ktran = 0;
    float C = 0;
    float emmision = 0;
protected:
private:
};


#endif /* material_hpp */
