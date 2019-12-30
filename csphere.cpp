#include "csphere.h"
#include "boundingbox.h"

CSphere::CSphere()
{
    //ctor
}
CSphere::CSphere(GVector3 c, double r)
{
    center = c;
    radius = r;
    max[0] = c.x + r;
    max[1] = c.y + r;
    max[2] = c.z + r;
    min[0] = c.x - r;
    min[1] = c.y - r;
    min[2] = c.z - r;
}
CSphere::CSphere(CSphere& s)
{
    center = s.getCenter();
    radius = s.getRadius();
    max[0] = this->center.x + this->radius;
    max[1] = this->center.y + this->radius;
    max[2] = this->center.z + this->radius;
    min[0] = this->center.x - this->radius;
    min[1] = this->center.y - this->radius;
    min[2] = this->center.z - this->radius;
}

CSphere::CSphere(GVector3 center, double radius, GVector3 x,GVector3 y,GVector3 z)
{
    this->center = center;
    this->radius = radius;
    this->xaxis = x;
    this->yaxis = y;
    this->zaxis = z;
    max[0] = this->center.x + this->radius;
    max[1] = this->center.y + this->radius;
    max[2] = this->center.z + this->radius;
    min[0] = this->center.x - this->radius;
    min[1] = this->center.y - this->radius;
    min[2] = this->center.z - this->radius;
}
CSphere::~CSphere()
{
    //dtor
}
void CSphere::setCenter(GVector3& c)
{
    center = c;
    max[0] = this->center.x + this->radius;
    max[1] = this->center.y + this->radius;
    max[2] = this->center.z + this->radius;
    min[0] = this->center.x - this->radius;
    min[1] = this->center.y - this->radius;
    min[2] = this->center.z - this->radius;
}
void CSphere::setRadius(double r)
{
    radius = r;
    max[0] = this->center.x + this->radius;
    max[1] = this->center.y + this->radius;
    max[2] = this->center.z + this->radius;
    min[0] = this->center.x - this->radius;
    min[1] = this->center.y - this->radius;
    min[2] = this->center.z - this->radius;
}
GVector3 CSphere::getCenter()
{
    return center;
}
double CSphere::getRadius()
{
    return radius;
}
GVector3 CSphere::getNormal(GVector3 p)
{
    return p - center;
}
inline IntersectResult CSphere::isIntersected(CRay _ray)
{
    IntersectResult result = IntersectResult::noHit();

    GVector3 v = _ray.getOrigin() - center;
    float a0 = v.dotMul(v) - radius*radius;
    float DdotV = _ray.getDirection().dotMul(v);

    if(abs(v.dotMul(v))< radius*radius){
        result.isHit = 1;
        //result.distance = (sqrt(v.dotMul(_ray.getDirection()) + radius*radius))*2;
        result.distance = (v.dotMul(_ray.getDirection().negate()))*2;
        std::cout<<result.distance<<std::endl;
        result.position = _ray.getPoint(result.distance);
        result.normal = result.position - center;
        //result.object = this;
        result.normal.normalize();

        GVector3 r = (result.position - this->getCenter())/this->radius;
        float v = (acos(this->yaxis.dotMul(r))/PI +1)/2;
        float u = ((atan2(r.dotMul(this->zaxis), r.dotMul(this->xaxis)))/PI + 1)/2;

        result.u = u;
        result.v = v;

//        result.color = this->phongMaterial->sample(_ray,result.position,result.normal);

        CObject *temp = this;
        PhongMaterial *tempM = this->phongMaterial;
        temp->phongMaterial = tempM;
        result.object = temp;
    }
    else if (DdotV <= 0) {
        float discr = DdotV * DdotV - a0;
        if (discr >= 0) {
            //
            result.isHit = 1;
            result.distance = -DdotV - sqrt(discr);
            result.position = _ray.getPoint(result.distance);
            result.normal = result.position - center;
            //result.object = this;
            result.normal.normalize();

            GVector3 r = (result.position - this->getCenter())/this->radius;
            float v = (acos(this->yaxis.dotMul(r))/PI);
            float u = ((atan2(r.dotMul(this->zaxis), r.dotMul(this->xaxis)))/PI + 1)/2;

            result.u = u;
            result.v = v;

           // result.color = this->phongMaterial->sample(_ray,result.position,result.normal);

            CObject *temp =this;
            PhongMaterial *tempM = this->phongMaterial;
            temp->phongMaterial = tempM;
            result.object = temp;
        }
    }
    return result;
}

bool CSphere::bounding_box(float t0, float t1, aabb& box){
    return true;
}

bool CSphere::hit(CRay ray, float t_min, float t_max, IntersectResult &result){
    return false;
}
