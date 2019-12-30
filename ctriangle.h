//
//  ctriangle.h
//  CS285_Assignment2_1.0
//
//  Created by 喀相明 on 2019/5/12.
//  Copyright © 2019 喀相明. All rights reserved.
//

#ifndef ctriangle_h
#define ctriangle_h

#include "gvector3.h"
#include "cray.h"
#include "cobject.h"
#include "material.h"

class CTriangle: public CObject
{
public:
    CTriangle();
    CTriangle(GVector3 point1, GVector3 point2, GVector3 point3 );
    CTriangle(CTriangle& s);
    void setPoints(GVector3& a,GVector3& b,GVector3& c);
    GVector3 getVertex1();
    GVector3 getVertex2();
    GVector3 getVertex3();
    GVector3 norm1;
    GVector3 norm2;
    GVector3 norm3;


    bool itpnorm = false;

    PhongMaterial* phongMaterial;


    virtual GVector3 getNormal(GVector3 point);

    virtual inline IntersectResult isIntersected(CRay RAY);
    virtual ~CTriangle();
protected:
private:
    GVector3 Vertex1;
    GVector3 Vertex2;
    GVector3 Vertex3;
};
#endif /* ctriangle_h */
