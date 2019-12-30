#ifndef GVECTOR3_H
#define GVECTOR3_H
#include <iostream>
#include <cmath>

using namespace std;


class GVector3
{

public:
    float x;
    float y;
    float z;
    // »± °ππ‘Ï∫Ø ˝
    GVector3();
    ~GVector3();
    // ”√ªßππ‘Ï∫Ø ˝
    GVector3(float posX, float posY, float posZ);
    // ‰≥ˆœÚ¡ø–≈œ¢
    void getInfo();
    // ∏¡øº”∑®
    GVector3 operator+(GVector3 v);
    // ∏¡øºı∑®
    GVector3 operator-(GVector3 v);
    // ˝≥À
    GVector3 operator*(float n);
    // ˝≥˝
    GVector3 operator/(float n);
    //œÚ¡øµ„ª˝
    float dotMul(GVector3 v2);
    //œÚ¡ø≤Ê≥À
    GVector3 crossMul(GVector3 v2);
    //æ¯∂‘÷µªØ
    GVector3 abs();
    //ªÒ»°∑÷¡ø÷–µƒ◊Ó¥Û÷µ
    float Vmax();
    //ªÒ»°∑÷¡øµƒ◊Ó–°÷µ
    float Vmin();
    //ªÒ»° ∏¡ø≥§∂»
    float getLength();
    //œÚ¡øµ•ŒªªØ
    GVector3 normalize();

    GVector3 negate();
    //«Û¡Ωµ„÷Æº‰µƒæ‡¿Î
    float getDist(GVector3 v);
    //∑µªÿ¡„œÚ¡ø
    static inline GVector3 zero() { return GVector3(0, 0, 0); }
    //¥Ú”°œÚ¡øµƒ∑÷¡ø÷µ
    void show();

};

class GColor
{
public:
    float r;
    float g;
    float b;
    GColor();
    GColor(float _r,float _g,float _b);
    GColor add(const GColor& c);
    GColor multiply(float s) const;
    GColor divide(float s) const;
    GColor modulate(const GColor& c) const;
    void saturate();
    void show();
    virtual ~GColor();
    static inline GColor black(){ return GColor(0,0,0); }
    static inline GColor white(){ return GColor(1,1,1); }
    static inline GColor red()  { return GColor(1,0,0); }
    static inline GColor green(){ return GColor(0,1,0); }
    static inline GColor blue() { return GColor(0,0,1); }
protected:
private:

};

#endif

