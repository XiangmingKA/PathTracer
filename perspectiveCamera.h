#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H
#include "cray.h"
#include "cobject.h"

class perspectiveCamera {
public:
    perspectiveCamera();
    ~perspectiveCamera();
    perspectiveCamera(const GVector3& _eye, const GVector3& _front, const GVector3& _refUp, float _fov);
    perspectiveCamera(const GVector3& _eye, const GVector3& _front, const GVector3& _refUp, float _fov,float focladistance);
    void setperspectiveCamera(const GVector3& _eye, const GVector3& _front, const GVector3& _refUp, float _fov);
    void setperspectiveCamera(const GVector3& _eye, const GVector3& _front, const GVector3& _refUp, float _fov,float focladistance);
    CRay generateRay(float x, float y);
    CRay generateRayUsingLen(float x, float y);
    GVector3 eye;

private:
    GVector3 front;
    GVector3 refUp;
    float   fov;
    GVector3 right;
    GVector3 up;
    float   fovScale;
    float focaldis;
    CPlane lenPlane;
    CPlane focalPlane;
    float R = 10;
    float m = 5;
};
#endif

inline float get_random_float();