#pragma once

#include <stdio.h>
#include "scene_io.h"

void newreadScene(SceneIO IO);

struct myMaterial {
    Color diffColor;    /* Diffuse color                */
    Color ambColor;    /* Ambient color                */
    Color specColor;    /* Specular color                 */
    Color emissColor;    /* Emissive color                */
    Flt shininess;    /* Shininess: 0.0 - 1.0.  Must be scaled (multiply by 128) before use as a Phong
					  cosine exponent (q in our equation).  */
    Flt ktran;        /* Transparency: 0.0 - 1.0            */
};

struct myLight {
    enum LightType type;  /* Point, directional, or spot light        */

    Point position;    /* Position of a point light source        */
    Vec direction;    /* Direction of a directional light source    */
    Color color;    /* Color and intensity                */

    Flt dropOffRate;    /* For spot lights, 0 <= x <= 1.  See man pages */
    Flt cutOffAngle;
    myLight *next;
};

struct mySphere {
    Point origin;
    Flt radius;
    Vec xaxis;
    Flt xlength;
    Vec yaxis;
    Flt ylength;
    Vec zaxis;
    Flt zlength;
    myMaterial *material;
    mySphere *next;
};

struct myTriangle {
    Point vecPositions[3];
    Vec normal;
    myMaterial *material;
    myTriangle *next;

};

struct myScene {
    myLight *lights;
    mySphere *spheres;
    myTriangle *triangles;
};

struct myCamera {
    Point viewPosition;
    Vec viewDirection;
    Vec upVector;
    Flt verticalFOV;
};

void newreadScene2();