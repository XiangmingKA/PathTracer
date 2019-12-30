#include "gvector3.h"
#include <iostream>
using namespace std;

GVector3::GVector3()
{
}
GVector3::~GVector3()
{
}
GVector3::GVector3(float posX, float posY, float posZ)
{
    x = posX;
    y = posY;
    z = posZ;
}

GVector3 GVector3::operator+(GVector3 v)
{
    return GVector3(x + v.x, v.y + y, v.z + z);
}
GVector3 GVector3::operator-(GVector3 v)
{
    return GVector3(x - v.x, y - v.y, z - v.z);
}
GVector3 GVector3::operator*(float n)
{
    return GVector3(x*n, y*n, z*n);
}
GVector3 GVector3::operator/(float n)
{
    return GVector3(x / n, y / n, z / n);
}
void GVector3::getInfo()
{
    cout << "x:" << x << " y:" << y << " z:" << z << endl;
}
GVector3 GVector3::abs()
{
    if (x<0) x *= -1;
    if (y<0) y *= -1;
    if (z<0) z *= -1;
    return GVector3(x, y, z);
}
float GVector3::dotMul(GVector3 v2)
{
    return (x*v2.x + y*v2.y + z*v2.z);
}
GVector3 GVector3::crossMul(GVector3 v2)
{
    GVector3 vNormal;
    // º∆À„¥π÷± ∏¡ø
    vNormal.x = ((y * v2.z) - (z * v2.y));
    vNormal.y = ((z * v2.x) - (x * v2.z));
    vNormal.z = ((x * v2.y) - (y * v2.x));
    return vNormal;
}
float GVector3::getLength()
{
    return  (float)sqrt(x*x + y*y + z*z);
}
GVector3 GVector3::normalize()
{
    float length = getLength();
    x = x / length;
    y = y / length;
    z = z / length;
    return GVector3(x, y, z);
}

GVector3 GVector3::negate(){
    x = -x;
    y = -y;
    z = -z;
    return GVector3(x, y, z);
}
void GVector3::show()
{
    cout << "x:" << x << "  y:" << y << "  z" << z << endl;
}
float GVector3::Vmax()
{
    float tmp = max(y, z);
    return max(x, tmp);
}
float GVector3::Vmin()
{
    float tmp = min(y, z);
    return min(x, tmp);
}
float GVector3::getDist(GVector3 v)
{
    float tmp = (x - v.x)*(x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z);
    return sqrt(tmp);
}


GColor::GColor()
{
    //ctor
}

GColor::~GColor()
{
    //dtor
}
GColor::GColor(float _r,float _g,float _b)
{
    r=_r;g=_g;b=_b;
}
GColor GColor::add(const GColor& c)
{
    return GColor(r + c.r, g + c.g, b + c.b);
}
GColor GColor::multiply(float s) const
{
    return GColor(r * s, g * s, b * s);
}

GColor GColor::divide(float s) const
{
    return GColor(r / s, g / s, b / s);
}

GColor GColor::modulate(const GColor&c) const
{
    return GColor(r * c.r, g * c.g, b * c.b);
}
void GColor::saturate()
{
    r = r>1.0?1.0:r;
    g = g>1.0?1.0:g;
    b = b>1.0?1.0:b;
}
void GColor::show()
{
    cout<<"r:"<<r<<"g:"<<g<<"b:"<<b<<endl;
}
