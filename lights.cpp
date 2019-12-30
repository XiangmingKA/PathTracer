//
//  lights.cpp
//  CS285_Assignment2_1.0
//
//  Created by 喀相明 on 2019/5/12.
//  Copyright © 2019 喀相明. All rights reserved.
//

#include "lights.h"
#include "stdio.h"
#include "global.h"


DirectLight::DirectLight()
{
    //ctor
}
DirectLight::DirectLight(GColor _color,GVector3 _direction,bool _isShadow)
{
    color=_color;
    direction=_direction.normalize();
    isShadow=_isShadow;
}
DirectLight::~DirectLight()
{
    //dtor
}
//通过光线与场景的相交结果计算光照结果
GColor DirectLight::intersect(Union &scence,IntersectResult &rayResult)
{
    //生产shadowRay的修正值
    const float k=0.0001;
    //生成与光照相反方向的shadowRay
    GVector3 shadowDir=direction.normalize().negate();
    CRay shadowRay=CRay(rayResult.position+rayResult.normal*k,shadowDir);
    //计算shadowRay是否与场景相交
    IntersectResult lightResult = intersectBVH(headNode , shadowRay );
    GColor resultColor = GColor::black();
    if(isShadow&&lightResult.isHit)
    {
        if(lightResult.object->phongMaterial->ktran==0&&lightResult.object->phongMaterial->C==0)
        {
            return GColor(0,0,0);
            //return resultColor;
        }
    }

    //计算光强
    float NdotL=rayResult.normal.dotMul(shadowDir);
    if (NdotL >= 0)
        resultColor=resultColor.add(this->color.multiply(NdotL));
    //return this->color;
    //std::cout<<"resultlight"<<resultColor.r<<","<<resultColor.g<<","<<resultColor.b<<std::endl;
    //std::cout<<"thisColor"<<this->color.r<<","<<this->color.g<<","<<this->color.b<<std::endl;
    //std::cout<<"NdotL"<<NdotL<<std::endl;

    return resultColor;
}

PointLight::PointLight()
{
    //ctor
}

PointLight::~PointLight()
{
    //dtor
}
PointLight::PointLight(GColor _color,GVector3 _position,bool _isShadow)
{
    color=_color;
    position=_position;
    isShadow=_isShadow;
}
//通过光线与场景的相交结果计算光照结果
GColor PointLight::intersect(Union &scence,IntersectResult &rayResult)
{
    //生产shadowRay的修正值
    const float k=0.0001;
    GVector3 delta=this->position-rayResult.position;
    float distance=delta.getLength();
    //生成与光照相反方向的shadowRay
    CRay shadowRay=CRay(rayResult.position+rayResult.normal*k,delta.normalize());//?
    //CRay shadowRay=CRay(rayResult.position,delta.normalize());
    GVector3 shadowDir=delta.normalize();
    //计算shadowRay是否与场景相交
    IntersectResult lightResult = intersectBVH(headNode , shadowRay );
    GColor resultColor = GColor::black();
    GColor returnColor= GColor::black();
    //如果shadowRay与场景中的物体相交
    if(lightResult.isHit&&(lightResult.distance<=distance))
    {
        //return resultColor;
        if(lightResult.object->phongMaterial->ktran==0)
            return GColor(0,0,0);
    }

    {
        resultColor=this->color.divide(distance*distance);
        float NdotL=rayResult.normal.dotMul(shadowDir);
        if (NdotL >= 0)
            returnColor=returnColor.add(resultColor.multiply(NdotL));           //+5
        return returnColor;
    }

}

Light:: Light(){

}

Light:: ~Light(){

}

GColor Light::intersect(Union &scence,IntersectResult &result){
    return GColor(0,0,0);
}

lightUnion::lightUnion()
{
    //ctor
}

lightUnion::~lightUnion()
{
    //dtor
}

void lightUnion::push(Light* light)

{
    lights.push_back(light);
}
GColor lightUnion::intersect(Union &scence,IntersectResult &result)
{
    GColor resColor(0,0,0);
    long size=this->lights.size();
    for (long i=0;i<size;i++){
        resColor.r += this->lights[i]->intersect(scence, result).r;
        resColor.g += this->lights[i]->intersect(scence, result).g;
        resColor.b += this->lights[i]->intersect(scence, result).b;

    }
    resColor.r = resColor.r > 1? 1 : resColor.r;
    resColor.g = resColor.g > 1? 1 : resColor.g;
    resColor.b = resColor.b > 1? 1 : resColor.b;
    resColor.r = resColor.r < 0? 0 : resColor.r;
    resColor.g = resColor.g < 0? 0 : resColor.g;
    resColor.b = resColor.b < 0? 0 : resColor.b;
    //std::cout<<resColor.r<<","<<resColor.g<<","<<resColor.b<<std::endl;
    return resColor;
}


GVector3 PointLight::getLightDir(GVector3 pos){
    return (pos - this->position).normalize();
}

GVector3 DirectLight::getLightDir(GVector3 pos){
    return this->direction;
}

GVector3 Light::getLightDir(GVector3 pos){
    return GVector3(0,0,0);
}
