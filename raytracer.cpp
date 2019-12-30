//Written by Xiangming KA
//All rights reserved

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <vector>
#include <utility>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <cmath>

#include "scene_io.h"
#include "myScene.h"
#include "cray.h"
#include "csphere.h"
#include "gvector3.h"
#include "perspectiveCamera.h"
#include "display.h"
#include "ctriangle.h"
#include "lights.h"
#include "global.h"
#include "cobject.h"


newFrameBuffer myBuffer(IMAGE_WIDTH, IMAGE_HEIGHT);
newFrameBuffer sampleBuffer(IMAGE_WIDTH*4, IMAGE_HEIGHT*4);

typedef unsigned char u08;

SceneIO *scene = NULL;

GColor pathTraceRecursive(CRay& ray,long maxReflect, int maxPathDeep, int countOfFra){
    IntersectResult result = intersectBVH(headNode , ray);

    if (result.isHit)
    {
        float reflectiveness = result.object->phongMaterial->getRef();
        float ktran = result.object->phongMaterial->ktran;
        ktran = 0;

        GColor color = result.object->phongMaterial->sample(ray, result.position, result.normal, &result);
        if ((ktran > 0) && (maxReflect > 0))
        {
            float n1 ;
            float n2 ;
            if(countOfFra % 2 ==0){
                n1 = 1.0;
                n2 = 1.5;
            }
            else
            {
                n1 = 1.5;
                n2 = 1;
            }
            color = color.multiply(0.01);
            GVector3 Q = (result.normal)*(result.normal.dotMul(ray.getDirection()));
            GVector3 M = (Q+ray.getDirection())*(n1/n2);
            GVector3 P = result.normal * (-cos(n2));
            GVector3 T = (M + P).normalize();
            GVector3 r = result.normal*(-2 * result.normal.dotMul(ray.getDirection()))+ray.getDirection();
            //GVector3 r = ray.getDirection();
            //CRay ray = CRay(result.position - result.normal*0.001, r);
            CRay ray = CRay(result.position - result.normal*0.0001, T);
            GColor ktransColor = pathTraceRecursive(ray, maxReflect - 1,maxPathDeep,countOfFra+1);
            color = color.add(ktransColor.multiply(0.5));
        }

        //if ((reflectiveness > 0) && (maxReflect > 0))
        if ((reflectiveness > 0) && (maxReflect > 0))
        {
            color = color.multiply(1 - reflectiveness);
            GVector3 r = result.normal*(-2 * result.normal.dotMul(ray.getDirection()))+ray.getDirection();
            CRay ray = CRay(result.position + result.normal*0.001, r);
            GColor reflectedColor = pathTraceRecursive(ray, maxReflect - 1,maxPathDeep,countOfFra);
            color = color.add(reflectedColor.multiply(reflectiveness));
        }
        //maxPathDeep > 0;
        else if(maxPathDeep > 0) {
            GColor resultColor(0,0,0);
            float u, v;
            GVector3 v1 = result.normal.crossMul(ray.getDirection()).normalize();
            GVector3 v2 = result.normal.crossMul(v1).normalize();
            GVector3 V ;
            GVector3 R ;
            for (int i = 0; i < 5; i++) {

            u = get_random_float()*2*PI;
            v = get_random_float()*PI/2;


            V = v2*sin(u) + v1*cos(u);
            R = (result.normal*sin(v) + V*cos(v)).normalize();

            CRay ray = CRay(result.position + result.normal*0.00001, R);

            resultColor = resultColor.add((pathTraceRecursive(ray, maxReflect - 1,maxPathDeep-1,countOfFra)).multiply(sin(v)));


            }
            //std::cout<<resultColor.r<<","<<resultColor.g<<","<<resultColor.b<<std::endl;
            color = color.multiply(0.3);
            color = color.add(resultColor.multiply(1.0/50.0));
        }
        color = color.multiply(2);
        return color;
    }else return GColor::black();

}


GColor rayTraceRecursive(CRay& ray,long maxReflect, int countOfFra)
{
    IntersectResult result = intersectBVH(headNode , ray );
    if (result.isHit)
    {
        float reflectiveness = result.object->phongMaterial->getRef();
        float ktran = result.object->phongMaterial->ktran;

        GColor color = result.object->phongMaterial->sample(ray, result.position, result.normal, &result);
        if ((ktran > 0) && (maxReflect > 0))
        {
            float n1 ;
            float n2 ;
            if(countOfFra % 2 ==0){
                n1 = 1.0;
                n2 = 1.5;
            }
            else
            {
                n1 = 1.5;
                n2 = 1;
            }
            color = color.multiply(0.01);
            GVector3 Q = (result.normal)*(result.normal.dotMul(ray.getDirection()));
            GVector3 M = (Q+ray.getDirection())*(n1/n2);
            GVector3 P = result.normal * (-cos(n2));
            GVector3 T = (M + P).normalize();
            //GVector3 r = result.normal*(-2 * result.normal.dotMul(ray.getDirection()))+ray.getDirection();
            GVector3 r = ray.getDirection();
            //CRay ray = CRay(result.position - result.normal*0.001, r);
            CRay ray = CRay(result.position - result.normal*0.00001, T);
            GColor ktransColor = rayTraceRecursive(ray, maxReflect - 1,countOfFra+1);
            color = color.add(ktransColor.multiply(1.2));
        }
        if ((reflectiveness > 0) && (maxReflect > 0))
        {
            color = color.multiply(1 - reflectiveness);
            GVector3 r = result.normal*(-2 * result.normal.dotMul(ray.getDirection()))+ray.getDirection();
            CRay ray = CRay(result.position + result.normal*0.001, r);
            GColor reflectedColor = rayTraceRecursive(ray, maxReflect - 1,countOfFra);
            color = color.add(reflectedColor.multiply(reflectiveness));
        }
        return color;
    }else return GColor::black();
}

void renderScene_PathTracing()
{

    float horiz = 0.0;
    float dep = 10;
    long maxDepth = 1000;

    long maxReflect = 4;
    int countOfRefra =0;
    float dx = 1.0f / IMAGE_WIDTH;
    float dy = 1.0f / IMAGE_HEIGHT;
    float dD = 255.0f / maxDepth;
    for (float y = 0; y < IMAGE_HEIGHT; ++y)
    {
        std::cout << "Rendering :" << y / IMAGE_HEIGHT * 100 << "%" << std::endl;
        float sy = 1 - dy*y;
        for (float x = 0; x < IMAGE_WIDTH; ++x)
        {
            float sx = dx*x;
            CRay ray(camera.generateRay(sx, sy));

            GColor color = pathTraceRecursive(ray, maxReflect,1,0);


            GVector3 screenPos ((x / myBuffer.width * 2 - 1),(1 - y / myBuffer.height * 2),0);
            GColor mycolor (color.b*255,color.g*255,color.r*255);

            myBuffer.set_pixel(screenPos, mycolor, 3);

        }
    }


}

void renderScene()
{

    float horiz = 0.0;
    float dep = 10;
    long maxDepth = 1000;

    long maxReflect = 4;
    int countOfRefra =0;
    float dx = 1.0f / IMAGE_WIDTH;
    float dy = 1.0f / IMAGE_HEIGHT;
    float dD = 255.0f / maxDepth;
    for (float y = 0; y < IMAGE_HEIGHT; ++y)
    {
        std::cout << "Rendering :" << y / IMAGE_HEIGHT * 100 << "%" << std::endl;
        float sy = 1 - dy*y;
        for (float x = 0; x < IMAGE_WIDTH; ++x)
        {
            float sx = dx*x;
            CRay ray(camera.generateRay(sx, sy));

                GColor color = rayTraceRecursive(ray, maxReflect,0);


                GVector3 screenPos ((x / myBuffer.width * 2 - 1),(1 - y / myBuffer.height * 2),0);
                GColor mycolor (color.b*255,color.g*255,color.r*255);

                myBuffer.set_pixel(screenPos, mycolor, 3);

        }
    }


}



void renderUsingDepthOfField()
{

    float horiz = 0.0;
    float dep = 10;
    long maxDepth = 1000;

    long maxReflect = 6;
    int countOfRefra =0;
    float dx = 1.0f / IMAGE_WIDTH;
    float dy = 1.0f / IMAGE_HEIGHT;
    float dD = 255.0f / maxDepth;
    for (float y = 0; y < IMAGE_HEIGHT; ++y)
    {
        std::cout << "Rendering :" << y / IMAGE_HEIGHT * 100 << "%" << std::endl;
        float sy = 1 - dy*y;
        for (float x = 0; x < IMAGE_WIDTH; ++x)
        {
            GColor color(0,0,0);
            float sx = dx*x;
            for(int s = 0; s<10;s++) {

                CRay ray(camera.generateRayUsingLen(sx, sy));

                GColor temp = rayTraceRecursive(ray, maxReflect, 0);
                color.r += temp.r;
                color.g += temp.g;
                color.b += temp.b;
                //color = rayTraceRecursive(&objects, ray, maxReflect,0);
                //std::cout<<"hit"<<std::endl;
            }
            GVector3 screenPos ((x / myBuffer.width * 2 - 1),(1 - y / myBuffer.height * 2),0);
            GColor mycolor (color.b*255/10,color.g*255/10,color.r*255/10);

            myBuffer.set_pixel(screenPos, mycolor, 3);

        }
    }


}





static void loadScene(char *name) {
    scene = readScene(name);
    newreadScene(*scene);

}




int main(int argc, char *argv[])
{

    loadScene("../Scenes/test1.ascii");               //load a ascii scene document


    buildBVHtree();                         //build BVH trees to accelerate path tracing
    //renderUsingDepthOfField();                     //Depth-of-field rendering using a len stracture
    renderScene_PathTracing();                     //normal path tracing rendering

    /******************************* outputing image **************************/
    /******************************* see ./out.ppm ****************************/
    std::ofstream ofs;
    ofs.open("./out.ppm");
    ofs << "P6\n" << IMAGE_WIDTH << " " << IMAGE_HEIGHT << "\n255\n";
    for (uint32_t i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT; ++i) {
        char r = (char)((myBuffer.getPixel(i)).b);
        char g = (char)((myBuffer.getPixel(i)).g);
        char b = (char)((myBuffer.getPixel(i)).r);
        ofs << r << g << b;
    }

    ofs.close();

    return 1;
}
