#ifndef CSPHERE_H
#define CSPHERE_H
#include "gvector3.h"
#include "material.h"
#include "cray.h"
#include "cobject.h"

class CSphere: public CObject
{
public:
    CSphere();
    CSphere(GVector3 center, double radius);
    CSphere(GVector3 center, double radius, GVector3 x,GVector3 y,GVector3 z);
    CSphere(CSphere& s);
    //Material* material;
    PhongMaterial* phongMaterial;
    void setCenter(GVector3& c);
    void setRadius(double r);
    GVector3 getCenter();
    double getRadius();

    virtual GVector3 getNormal(GVector3 point);

    virtual IntersectResult isIntersected(CRay RAY);
    virtual bool hit(CRay ray, float t_min, float t_max , IntersectResult& result);
    virtual bool bounding_box(float t0, float t1, aabb& box);
    virtual ~CSphere();
protected:
private:
    GVector3 center;
    GVector3 xaxis;
    GVector3 yaxis;
    GVector3 zaxis;
    double radius;
};

#endif

