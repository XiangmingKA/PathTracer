//
//  material.cpp
//  CS285_Assignment2_1.0
//
//  Created by 喀相明 on 2019/5/12.
//  Copyright © 2019 喀相明. All rights reserved.
//

#include "material.h"
#include "lights.h"
#include <stdio.h>
#include <iostream>
#include "global.h"

lightUnion lights;
Union objects;
perspectiveCamera camera;
using namespace std;


Material::Material()
{
    //ctor
}

Material::Material(float _reflectiveness)
{
    reflectiveness=_reflectiveness;
}
Material::~Material()
{
    //dtor
}
float Material::getRef()
{
    //return reflectiveness;
    return 0.2;
}
void Material::setRef(float _reflectiveness)
{
    reflectiveness=_reflectiveness;
}
/*
GColor Material::sample(const CRay& ray,const GVector3& position,const GVector3& normal)
{
    cout<<"Base sample!"<<endl;
    GColor res;
    res.white();
    return res;
}
*/
PhongMaterial::PhongMaterial()
{
    //ctor
}
PhongMaterial::PhongMaterial(const GColor& _diffuse,const GColor& _specular,const float& _shininess,float _reflectiveness)
{
    diffuse=_diffuse;
    specular=_specular;
    shininess=_shininess;
    reflectiveness=_reflectiveness;
}
PhongMaterial::PhongMaterial(const GColor& _ambient, const GColor& _diffuse,const GColor& _specular,const float& _shininess,float _reflectiveness, float Ktran)
{
    ambient = _ambient;
    diffuse=_diffuse;
    specular=_specular;
    shininess=_shininess;
    reflectiveness=_reflectiveness;
    ktran = Ktran;
}
PhongMaterial::~PhongMaterial()
{
    //dtor
}
GColor PhongMaterial::sample(CRay& ray,GVector3& position,GVector3& normal,IntersectResult* tempRes )
{

    GVector3 H = (lightDir-ray.getDirection()).normalize();
    float NdotH = normal.dotMul(H);
    //GColor diffuseTerm = this->diffuse.multiply(std::max(abs(NdotL), (float)0));
    GColor diffuseTerm = this->diffuse.multiply(1);
    //std::cout<<diffuseTerm.r<<", "<<diffuseTerm.g<<", "<<diffuseTerm.b<<std::endl;
    GColor specularTerm = this->specular.multiply(pow(std::max(abs(NdotH), (float)0), this->shininess*128));//128

    float u = tempRes->u;
    float v = tempRes->v;
/*
    if(this->usingInterp){
        diffuseTerm = (((v3Color.multiply(u)).add(v1Color.multiply(v))).add(v3Color.multiply(1-u-v)));
        return diffuseTerm.multiply(0.5);
    }
*/

//2020：outscatter
    if(0){
        float dens = 0.01;
        CRay innerRay(tempRes->position + ray.getDirection() *0.01, ray.getDirection());
        IntersectResult innerPoint  = intersectBVH(headNode,innerRay);
        GColor absortion(0,0,0);
        GColor outterColor;
        if(innerPoint.isHit) {
            //std::cout<<innerPoint.distance<<std::endl;
            if (innerPoint.object->phongMaterial->C == 0) {
                GColor innerColor = innerPoint.object->phongMaterial->sampleOutter(innerRay, innerPoint.position,
                                                                                   innerPoint.normal, &innerPoint);
                absortion = innerColor.multiply(
                        (1 - dens * innerPoint.distance) > 0 ? (1 - dens * innerPoint.distance) : 0);

            }
            else{
                CRay outterRay(innerPoint.position + ray.getDirection()*0.01, ray.getDirection());
                IntersectResult outterPoint  = intersectBVH(headNode,outterRay);
                GVector3 temp = outterPoint.normal.negate();

                if(outterPoint.isHit) {
                    outterColor = outterPoint.object->phongMaterial->sampleOutter(outterRay,outterPoint.position,temp, &outterPoint);
                    //outterColor = outterPoint.object->phongMaterial->diffuse;
                    //outterColor = outterColor.white();
                }
                else
                    outterColor = outterColor.black();

                GColor lightColor(0.5,0.5,0.5);
                lightColor = lightColor.multiply(innerPoint.distance*2);
                outterColor = outterColor.multiply((1 - dens*innerPoint.distance)>0? (1 - dens*innerPoint.distance):0) ;
                //absortion = outterColor.multiply((1 - 0.3*innerPoint.distance)>0? (1 - 0.3*innerPoint.distance):0) ;
                absortion = outterColor.multiply((1 - dens*innerPoint.distance)>0? (1 - dens*innerPoint.distance):0) ;
                //std::cout<<1-C*innerPoint.distance<<std::endl;

            }
        }
        else{
            return GColor(0,0,0);
        }
        //return absortion;
        GColor lightColor(0.5,0.5,0.5);
        lightColor = lightColor.multiply(innerPoint.distance*2);
        outterColor = outterColor.multiply((1 - 0.1*innerPoint.distance)>0? (1 - 0.1*innerPoint.distance):0) ;
        //std::cout<<1-C*innerPoint.distance<<std::endl;
        //return outterColor;
        diffuseTerm = diffuseTerm.multiply(0.6);

        /*
        outterColor.r += diffuseTerm.r;
        outterColor.g += diffuseTerm.g;
        outterColor.b += diffuseTerm.b;
*/
        //outterColor = outterColor.black();
        float density = 10;
        GColor inscratter(0,0,0);
        GColor sumScatter(0,0,0);
        float samples = 20;
        float deltaDis = 1;
        int i=0;
        for(; deltaDis*i < innerPoint.distance/5 ; i++){
            outterColor = (lightColor.multiply((1 - 0.7*i*deltaDis)>0? (1 - 0.7*i*deltaDis):0)).add(outterColor);
            inscratter = inscratter.black();
            //inscatter
            float u=0, v=0;
            for(int j = 0; j<samples; j++){
                u = get_random_float()*2*PI;
                v = get_random_float()*2*PI;

                GVector3 dir = ((GVector3(-1,0,0)*sin(u) + GVector3(0,-1,0)*cos(u)) + (GVector3(0,0,-1)*sin(v) + GVector3(0,-1,0)*cos(v))).normalize();

                CRay testray(innerRay.getPoint(deltaDis*i),dir);
                IntersectResult outpoint  = intersectBVH(headNode,testray);
                if(outpoint.isHit)
                inscratter = inscratter.add((outpoint.object->phongMaterial->sampleOutter(testray,outpoint.position,outpoint.normal,&outpoint).multiply
                        ((1 - density*outpoint.distance)>0? (1 - density*outpoint.distance):0)));
            }
            if(inscratter.r == 0.0 && inscratter.g == 0.0&&inscratter.b == 0.0)
                break;
            inscratter = inscratter.multiply(1/samples);
            inscratter.r = inscratter.r > 0? inscratter.r:0;
            inscratter.g = inscratter.g > 0? inscratter.g:0;
            inscratter.b = inscratter.b > 0? inscratter.b:0;
            //sumScatter = sumScatter.add(inscratter);
            sumScatter = sumScatter.add(inscratter.multiply((1 - density*i*deltaDis)>0? (1 - density*i*deltaDis):0));
            //std::cout<<sumScatter.r<<" , "<<sumScatter.g<<" , "<<sumScatter.b<<std::endl;
        }
        if(i!=0) {
            sumScatter.multiply((1 / i) * innerPoint.distance);
            outterColor.multiply((1 / i) * innerPoint.distance);
        }
        outterColor.r *= diffuseTerm.r;
        outterColor.g *= diffuseTerm.g;
        outterColor.b *= diffuseTerm.b;
        //sumScatter.multiply(50);
        return  sumScatter;
        //return  sumScatter.add(absortion);
        //return  lightColor;
        //return outterColor;

    }


    GColor lightStrength = lights.intersect(objects,*tempRes);

    GColor resColor (0,0,0);

    resColor.r = lightColor.r * (diffuseTerm.r + specularTerm.r) * lightStrength.r  + ambient.r;
    resColor.g = lightColor.g * (diffuseTerm.g + specularTerm.g) * lightStrength.g  + ambient.g;
    resColor.b = lightColor.b * (diffuseTerm.b + specularTerm.b) * lightStrength.b  + ambient.b;

    //resColor = diffuseTerm;

    return resColor;
}

GColor PhongMaterial::sampleOutter(CRay& ray,GVector3& position,GVector3& normal,IntersectResult* tempRes )
{

    GVector3 H = (lightDir-ray.getDirection()).normalize();
    float NdotH = normal.dotMul(H);
    //GColor diffuseTerm = this->diffuse.multiply(std::max(abs(NdotL), (float)0));
    GColor diffuseTerm = this->diffuse.multiply(1);
    //std::cout<<diffuseTerm.r<<", "<<diffuseTerm.g<<", "<<diffuseTerm.b<<std::endl;
    GColor specularTerm = this->specular.multiply(pow(std::max(abs(NdotH), (float)0), this->shininess*128));//128

    float u = tempRes->u;
    float v = tempRes->v;

    GColor lightStrength = lights.intersect(objects,*tempRes);
    //GColor lightStrength(0.2,0.2,0.2);

    GColor resColor (0,0,0);

    resColor.r = lightColor.r * (diffuseTerm.r + specularTerm.r) * lightStrength.r  + ambient.r;
    resColor.g = lightColor.g * (diffuseTerm.g + specularTerm.g) * lightStrength.g  + ambient.g;
    resColor.b = lightColor.b * (diffuseTerm.b + specularTerm.b) * lightStrength.b  + ambient.b;

    //resColor = diffuseTerm;
    //std::cout<<resColor.r<<resColor.g<<resColor.b<<std::endl;
    return resColor;
}

GColor PhongMaterial::sample(CRay& ray,GVector3& position,GVector3& normal)
{

    GVector3 H = (lightDir-ray.getDirection()).normalize();
    float NdotH = normal.dotMul(H);
    //GColor diffuseTerm = this->diffuse.multiply(std::max(abs(NdotL), (float)0));
    GColor diffuseTerm = this->diffuse.multiply(3);
    //std::cout<<diffuseTerm.r<<", "<<diffuseTerm.g<<", "<<diffuseTerm.b<<std::endl;
    GColor specularTerm = this->specular.multiply(pow(std::max(abs(NdotH), (float)0), this->shininess*128));//128
    //std::cout<<specularTerm.r<<", "<<specularTerm.g<<", "<<specularTerm.b<<std::endl;
    IntersectResult tempRes = objects.isIntersected(ray);

/*
    diffuseTerm = myTexture1[((int)(tempRes.u*600)+((int)(tempRes.v*600))*600)];
    diffuseTerm.r =0.5;
    diffuseTerm.g =0.5;
    diffuseTerm.b =0.5;


    if((((int)((tempRes.u)*100)/10)%2 == 0)&&(((int)((tempRes.v)*100)/10)%2 == 1)){
        diffuseTerm = GColor::black();
    }
    else{
        diffuseTerm = GColor::white();
    }

    return diffuseTerm;
    */
    GColor lightStrength = lights.intersect(objects,tempRes);

    GColor resColor (0,0,0);

    resColor.r = lightColor.r * (diffuseTerm.r + specularTerm.r) * lightStrength.r  + ambient.r;
    resColor.g = lightColor.g * (diffuseTerm.g + specularTerm.g) * lightStrength.g  + ambient.g;
    resColor.b = lightColor.b * (diffuseTerm.b + specularTerm.b) * lightStrength.b  + ambient.b;

    //resColor = diffuseTerm;

    return resColor;
}

GColor PhongMaterial::sample(CRay& ray,GVector3& position,GVector3& normal, float u, float v)
{
    float NdotL = normal.dotMul(lightDir);
    GVector3 H = (lightDir-ray.getDirection()).normalize();
    float NdotH = normal.dotMul(H);
    //GColor diffuseTerm = this->diffuse.multiply(std::max(abs(NdotL), (float)0));
    GColor diffuseTerm = this->diffuse.multiply(3);
    //std::cout<<diffuseTerm.r<<", "<<diffuseTerm.g<<", "<<diffuseTerm.b<<std::endl;
    GColor specularTerm = this->specular.multiply(pow(std::max(abs(NdotH), (float)0), this->shininess*128));//128
    //std::cout<<specularTerm.r<<", "<<specularTerm.g<<", "<<specularTerm.b<<std::endl;
    IntersectResult tempRes = objects.isIntersected(ray);
    /*
     diffuseTerm = myTexture1[((int)(tempRes.u*1024)+((int)(tempRes.v*512))*1024)];
     diffuseTerm.r /=255;
     diffuseTerm.g /=255;
     diffuseTerm.b /=255;
     */
    /*
     if((((int)((tempRes.u)*100)/10)%2 == 0)&&(((int)((tempRes.v)*100)/10)%2 == 1)){
     diffuseTerm = GColor::black();
     }
     else{
     diffuseTerm = GColor::white();
     }

     return diffuseTerm;
     */
    if(this->usingInterp){
        //diffuseTerm = (((v2Color.multiply(u)).add(v3Color.multiply(v))).add(v1Color.multiply(1-u-v))).multiply(2);

    }

    GColor lightStrength = lights.intersect(objects,tempRes);

    GColor resColor (0,0,0);

    resColor.r = lightColor.r * (diffuseTerm.r + specularTerm.r) * lightStrength.r  + ambient.r;
    resColor.g = lightColor.g * (diffuseTerm.g + specularTerm.g) * lightStrength.g  + ambient.g;
    resColor.b = lightColor.b * (diffuseTerm.b + specularTerm.b) * lightStrength.b  + ambient.b;

    //resColor = diffuseTerm;

    return resColor;
}

GColor PhongMaterial::sample(CRay& ray,GVector3& position,GVector3& normal, Light light){
    float NdotL = normal.dotMul(light.getLightDir(position));
    GVector3 H = (light.getLightDir(position)-ray.getDirection()).normalize();
    float NdotH = normal.dotMul(H);
    GColor diffuseTerm = this->diffuse.multiply(std::max(abs(NdotL), (float)0));
    //std::cout<<diffuseTerm.r<<", "<<diffuseTerm.g<<", "<<diffuseTerm.b<<std::endl;
    GColor specularTerm = this->specular.multiply(pow(std::max(abs(NdotH), (float)0), this->shininess*128));
    //std::cout<<specularTerm.r<<", "<<specularTerm.g<<", "<<specularTerm.b<<std::endl;
    IntersectResult tempRes = objects.isIntersected(ray);
    GColor lightStrength = lights.intersect(objects,tempRes);
    //std::cout<<lightStrength.r<<", "<<lightStrength.g<<", "<<lightStrength.b<<std::endl;
    lightStrength.saturate();

    diffuseTerm.r =0.5;
    diffuseTerm.g =0.5;
    diffuseTerm.b =0.5;
    GColor resColor (0,0,0);
    resColor.r = lightColor.r * (diffuseTerm.r + specularTerm.r) * lightStrength.r + ambient.r;
    resColor.g = lightColor.g * (diffuseTerm.g + specularTerm.g) * lightStrength.g + ambient.g;
    resColor.b = lightColor.b * (diffuseTerm.b + specularTerm.b) * lightStrength.b + ambient.b;
    return resColor;
    //return (lightColor.modulate(diffuseTerm.add(specularTerm))).multiply(lightStrength.r*2).add(ambient);
}
