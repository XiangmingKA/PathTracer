#include "perspectiveCamera.h"
#include "stdlib.h"
#include <iostream>
#include <cmath>
#include <random>

perspectiveCamera::perspectiveCamera()
{

}
perspectiveCamera::~perspectiveCamera()
{
}
perspectiveCamera::perspectiveCamera(const GVector3& _eye, const GVector3& _front, const GVector3& _refUp, float _fov)
{
    eye = _eye;
    front = _front;
    refUp = _refUp;
    fov = _fov;
    right = front.crossMul(refUp);
    up = right.crossMul(front);
    //fovScale = tan(fov* (PI  * 0.5f / 180)) * 2;
    fovScale = _fov;
}

void perspectiveCamera::setperspectiveCamera(const GVector3& _eye, const GVector3& _front, const GVector3& _refUp, float _fov)
{
    eye = _eye;
    front = _front;
    refUp = _refUp;
    fov = _fov;
    right = front.crossMul(refUp);
    up = right.crossMul(front);
    //fovScale = tan(fov* (PI  * 0.5f / 180)) * 2;
    fovScale = _fov;
    lenPlane = CPlane(front.negate(),m);
    focalPlane = CPlane(front.negate(),m+focaldis);
}

void perspectiveCamera::setperspectiveCamera(const GVector3& _eye, const GVector3& _front, const GVector3& _refUp, float _fov, float dis)
{
    eye = _eye;
    front = _front;
    refUp = _refUp;
    fov = _fov;
    right = front.crossMul(refUp);
    up = right.crossMul(front);
    //fovScale = tan(fov* (PI  * 0.5f / 180)) * 2;
    fovScale = _fov;
    focaldis = dis;
    lenPlane = CPlane(front.negate(),m);
    focalPlane = CPlane(front.negate(),m+focaldis);
}

perspectiveCamera::perspectiveCamera(const GVector3& _eye, const GVector3& _front, const GVector3& _refUp, float _fov,float focladistance){
    eye = _eye;
    front = _front;
    refUp = _refUp;
    fov = _fov;
    right = front.crossMul(refUp);
    up = right.crossMul(front);
    fovScale = _fov;
    focaldis = focladistance;
    lenPlane = CPlane(front.negate(),m);
    focalPlane = CPlane(front.negate(),m+focladistance);
}

CRay perspectiveCamera::generateRay(float x, float y)
{
    GVector3 r = right*((x - 0.5f) * fovScale);
    GVector3 u = up*((y - 0.5f) * fovScale);
    GVector3 tmp = front + r + u;
    tmp.normalize();
    return CRay(eye, tmp);
}

inline float get_random_float()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<float> dist(0.f, 1.f); // distribution in range [1, 6]

    return dist(rng);
}

CRay perspectiveCamera::generateRayUsingLen(float x, float y)
{
    GVector3 r = (right*((x - 0.5f))*R).negate();
    GVector3 u = (up*(y - 0.5f)*R).negate();
    GVector3 imagePlanePoint = r+u+this->eye;
    GVector3 dirP = ((front*m + eye) - imagePlanePoint).normalize();
    //GVector3 dirP = ((front*m +u + r).normalize());
    //CRay pray(eye, dirP);
    CRay pray(imagePlanePoint, dirP);
    IntersectResult focPlanePoint = focalPlane.isIntersected(pray);
    srand(time(0));
    GVector3 sr = right*R*((float)get_random_float()*2-1);
    //std::cout<<get_random_float()<<std::endl;
    srand(time(0));
    GVector3 su = up*R*((float)get_random_float()*2-1);
    GVector3 S = (eye+front*m) + sr + su;
    GVector3 sdir = (focPlanePoint.position - S).normalize();
    return CRay(S, sdir);
    //return CRay(imagePlanePoint, dirP);
}
