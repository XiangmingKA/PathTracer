//
//  cobject.h
//  CS285_Assignment2_1.0
//
//  Created by 喀相明 on 2019/5/12.
//  Copyright © 2019 喀相明. All rights reserved.
//

#ifndef cobject_h
#define cobject_h

#include <stdio.h>
#include "cray.h"
#include <vector>
#include "material.h"
#include "boundingbox.h"

using namespace std;
struct IntersectResult;
class Material;
class PhongMaterial;
class aabb;

class CObject
{
public:
    CObject();
    virtual ~CObject();
    //Material* material;
    PhongMaterial* phongMaterial;
    virtual IntersectResult isIntersected(CRay RAY);
    float max[3];
    float min[3];
    virtual bool hit(CRay ray, float t_min, float t_max , IntersectResult& result);
    virtual bool bounding_box(float t0, float t1, aabb& box);
};


class Union:public CObject
{
public:
    Union();
    virtual ~Union();
    void push(CObject* object);
    virtual inline IntersectResult isIntersected(CRay RAY);

    vector<CObject*> cobjects;

protected:
private:
};

struct IntersectResult {
    float distance;
    float u =0;
    float v =0;
    bool isHit;
    GVector3 position;
    GVector3 normal;
    CObject* object;
    //GColor color;
    static inline IntersectResult noHit() { return IntersectResult(); }
};

class myBvhNode {
public:
    myBvhNode();
    virtual ~myBvhNode();

    //myBvhNode(CObject *l, int n , float time0 , float time1);
    bool isLeaf;
    //virtual IntersectResult hit(CRay RAY);
    myBvhNode *left;
    myBvhNode *right;
    float max[3];
    float min[3];
    vector<aabb*> aabbBoxes;
};

class CPlane : public CObject{
public:
    GVector3 norm;
    float distance;
    CPlane();
    CPlane(GVector3 norm, float dis);
    //Material* material;
    PhongMaterial* phongMaterial;
    virtual IntersectResult isIntersected(CRay RAY);
    //virtual bool hit(CRay ray, float t_min, float t_max , IntersectResult& result);
    //virtual bool bounding_box(float t0, float t1, aabb& box);
    virtual ~CPlane();
};

void buildBVHtree();

IntersectResult intersectBVH(myBvhNode* head , CRay Ray );

extern std::vector<GColor> myTexture1;

extern std::vector<GColor> myTexture2;



#endif /* cobject_h */

