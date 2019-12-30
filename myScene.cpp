//
//  myScene.cpp
//  CS285_Assignment2_1.0
//
//  Created by 喀相明 on 2019/5/10.
//  Copyright © 2019 喀相明. All rights reserved.
//

#include "myScene.h"
#include "scene_io.h"
#include "cobject.h"
#include "perspectiveCamera.h"
#include "ctriangle.h"
#include "csphere.h"
#include "lights.h"
#include <cstring>
#include "material.h"
#include "global.h"


int numberOfObjects = 0;
float reflection = 0;


void newreadScene(SceneIO IO)
{
    //read camera
    GVector3 cameraPos;
    GVector3 viewDirection;
    GVector3 orthoUp;
    float verticalFov;
    cameraPos.x = IO.camera->position[0];
    cameraPos.y = IO.camera->position[1];
    cameraPos.z = IO.camera->position[2];
    viewDirection.x = IO.camera->viewDirection[0];
    viewDirection.y = IO.camera->viewDirection[1];
    viewDirection.z = IO.camera->viewDirection[2];
    orthoUp.x = IO.camera->orthoUp[0];
    orthoUp.y = IO.camera->orthoUp[1];
    orthoUp.z = IO.camera->orthoUp[2];
    verticalFov = IO.camera->verticalFOV;

    //camera.setperspectiveCamera(cameraPos, viewDirection,orthoUp, verticalFov);
    camera.setperspectiveCamera(cameraPos, viewDirection,orthoUp, verticalFov,IO.camera->focalDistance);

    //read objects

    while (IO.objects != NULL) {
        if (IO.objects->type == POLYSET_OBJ) {

            PolySetIO *pset = (PolySetIO *)IO.objects->data;
            PolygonIO *poly;
            VertexIO *vert;

            poly = pset->poly;
            vert = poly->vert;



            MaterialIO *material = IO.objects->material;
            GColor ambColor;
            ambColor.r = material->ambColor[0];
            ambColor.g = material->ambColor[1];
            ambColor.b = material->ambColor[2];
            GColor difColor;
            difColor.r = material->diffColor[0];
            difColor.g = material->diffColor[1];
            difColor.b = material->diffColor[2];
            GColor speColor;
            speColor.r = material->specColor[0];
            speColor.g = material->specColor[1];
            speColor.b = material->specColor[2];
            float shininess = material->shininess;
            float ktran = material->ktran;

            //PhongMaterial *tempMaterial = new PhongMaterial(ambColor,difColor, speColor, shininess,reflection,ktran);
            PhongMaterial *tempMaterial = new PhongMaterial(ambColor,difColor, speColor, shininess,0.0,ktran);
            //new

            /*
            if(ktran != 0){
                tempMaterial->C = 0.2;
                tempMaterial->ktran = 0;
            }
             */
             /*
            if(reflection !=0){
                tempMaterial->C = 0;
            }
            else{
                tempMaterial->C = 0.2;
            }
*/
            for (int i = 0; i < pset->numPolys; i++, poly++) {
                vert = poly->vert;
                GVector3 vertex1;
                GVector3 vertex2;
                GVector3 vertex3;
                GVector3 norm1;
                GVector3 norm2;
                GVector3 norm3;
                GColor color1;
                GColor color2;
                GColor color3;
                MaterialIO tempmaterial= IO.objects->material[vert->materialIndex];

                vertex1.x = vert->pos[0];
                vertex1.y = vert->pos[1];
                vertex1.z = vert->pos[2];
                norm1.x = vert->norm[0];
                norm1.y = vert->norm[1];
                norm1.z = vert->norm[2];
                color1.r = tempmaterial.diffColor[0];
                color1.g = tempmaterial.diffColor[1];
                color1.b = tempmaterial.diffColor[2];
                vert++;

                vertex2.x = vert->pos[0];
                vertex2.y = vert->pos[1];
                vertex2.z = vert->pos[2];
                norm2.x = vert->norm[0];
                norm2.y = vert->norm[1];
                norm2.z = vert->norm[2];
                tempmaterial= IO.objects->material[vert->materialIndex];

                tempmaterial = *material;

                color2.r = tempmaterial.diffColor[0];
                color2.g = tempmaterial.diffColor[1];
                color2.b = tempmaterial.diffColor[2];


                vert++;
                vertex3.x = vert->pos[0];
                vertex3.y = vert->pos[1];
                vertex3.z = vert->pos[2];
                norm3.x = vert->norm[0];
                norm3.y = vert->norm[1];
                norm3.z = vert->norm[2];

                tempmaterial = *material;
                tempmaterial= IO.objects->material[vert->materialIndex];
                color3.r = tempmaterial.diffColor[0];
                color3.g = tempmaterial.diffColor[1];
                color3.b = tempmaterial.diffColor[2];


                CTriangle* tempTringle = new CTriangle(vertex1, vertex2, vertex3);
                /*
                if(pset->normType == PER_VERTEX_NORMAL){
                    tempTringle->norm1 = norm1;
                    tempTringle->norm2 = norm1;
                    tempTringle->norm3 = norm1;
                    tempTringle->itpnorm = true;
                }
                 */
                tempMaterial->v1Color = color1;
                tempMaterial->v2Color = color2;
                tempMaterial->v3Color = color3;
                if(IO.objects->numMaterials != 1)
                    tempMaterial->usingInterp = true;

                tempTringle->phongMaterial = tempMaterial;
                objects.push(tempTringle);
                std::cout << "read triangle"<<std::endl;

                //read material
                objects.cobjects[numberOfObjects++]->phongMaterial = tempMaterial;
            }
        }
        else if (IO.objects->type == SPHERE_OBJ) {

            SphereIO *sphere = (SphereIO *)IO.objects->data;

            GVector3 center;
            center.x = sphere->origin[0];
            center.y = sphere->origin[1];
            center.z = sphere->origin[2];

            float radius = sphere->radius;

            MaterialIO *material = IO.objects->material;

            GColor ambColor;
            ambColor.r = material->ambColor[0];
            ambColor.g = material->ambColor[1];
            ambColor.b = material->ambColor[2];
            GColor difColor;
            difColor.r = material->diffColor[0];
            difColor.g = material->diffColor[1];
            difColor.b = material->diffColor[2];
            GColor speColor;
            speColor.r = material->specColor[0];
            speColor.g = material->specColor[1];
            speColor.b = material->specColor[2];

            GVector3 xAxis;
            xAxis.x = sphere->xaxis[0];
            xAxis.y = sphere->xaxis[1];
            xAxis.z = sphere->xaxis[2];

            GVector3 yAxis;
            yAxis.x = sphere->yaxis[0];
            yAxis.y = sphere->yaxis[1];
            yAxis.z = sphere->yaxis[2];

            GVector3 zAxis;
            zAxis.x = sphere->zaxis[0];
            zAxis.y = sphere->zaxis[1];
            zAxis.z = sphere->zaxis[2];


            float shininess = material->shininess;
            float ktran = material->ktran;

            //PhongMaterial *tempMaterial = new PhongMaterial(ambColor,difColor, speColor, shininess,reflection,ktran);
            PhongMaterial *tempMaterial = new PhongMaterial(ambColor,difColor, speColor, shininess,0.0,ktran);
            /*
            if(ktran != 0){
                tempMaterial->C = 0.2;
                tempMaterial->ktran = 0;
            }*/

            CSphere* tempSphere = new CSphere(center, radius, xAxis,yAxis,zAxis);
            //CSphere* tempSphere = new CSphere(center, radius);
            tempSphere->phongMaterial = tempMaterial;
            objects.push(tempSphere);

            //read material
            objects.cobjects[numberOfObjects++]->phongMaterial = tempMaterial;

        }
        std::cout << "read object" << std::endl;
        IO.objects = IO.objects->next;
    }
    //read lights
    while (IO.lights != NULL) {
        if (IO.lights->type == POINT_LIGHT)
        {

            GVector3 lightPos;
            lightPos.x = IO.lights->position[0];
            lightPos.y = IO.lights->position[1];
            lightPos.z = IO.lights->position[2];

            GColor lightColor;
            lightColor.r = IO.lights->color[0];
            lightColor.g = IO.lights->color[1];
            lightColor.b = IO.lights->color[2];

            PointLight *templight = new PointLight(lightColor, lightPos, true);
            lights.push(templight);

        }
        else if (IO.lights->type == DIRECTIONAL_LIGHT) {
            GVector3 lightDir;
            lightDir.x = IO.lights->direction[0];
            lightDir.y = IO.lights->direction[1];
            lightDir.z = IO.lights->direction[2];

            GColor lightColor;
            lightColor.r = IO.lights->color[0];
            lightColor.g = IO.lights->color[1];
            lightColor.b = IO.lights->color[2];

            DirectLight *templight = new DirectLight(lightColor, lightDir, true);
            lights.push(templight);

        }
        std::cout << "read light" << std::endl;
        IO.lights = IO.lights->next;
    }


}

void newreadScene2(){
    GVector3 cameraPos;
    GVector3 viewDirection;
    GVector3 orthoUp;
    float verticalFov;
    cameraPos.x = 278;
    cameraPos.y = 278;
    cameraPos.z = -800;
    viewDirection.x = 1;
    viewDirection.y = 1;
    viewDirection.z = 0;
    orthoUp.x = 0;
    orthoUp.y = 1;
    orthoUp.z = 0;
    verticalFov = 40.0;

    //camera.setperspectiveCamera(cameraPos, viewDirection,orthoUp, verticalFov);
    camera.setperspectiveCamera(cameraPos, viewDirection,orthoUp, verticalFov,10);
    GColor ambColor(255,255,255);
    GColor difColor(255,255,255);
    GColor speColor(255,255,255);
    PhongMaterial *tempMaterial = new PhongMaterial(ambColor,difColor, speColor, 2,0.2,0);

    CTriangle* Tri1 = new CTriangle(GVector3(0,0,555),GVector3(555,0,555),GVector3(555,555,555));
    Tri1->phongMaterial = tempMaterial;
    CTriangle* Tri2 = new CTriangle(GVector3(0,0,555),GVector3(0,555,555),GVector3(555,555,555));
    Tri2->phongMaterial = tempMaterial;
    CTriangle* Tri3 = new CTriangle(GVector3(0,0,0),GVector3(555,0,0),GVector3(555,555,0));
    Tri3->phongMaterial = tempMaterial;
    CTriangle* Tri4 = new CTriangle(GVector3(0,0,0),GVector3(0,555,0),GVector3(555,555,0));
    Tri4->phongMaterial = tempMaterial;

    objects.push(Tri1);
    objects.push(Tri2);
    objects.push(Tri3);
    objects.push(Tri4);
}


/*
 void myreadScene(SceneIO IO){

 myLight *lightHead = Scene.lights;
 myTriangle *triangleHead = Scene.triangles;
 mySphere *sphereHead = Scene.spheres;

 //read camera
 memcpy(Camera.upVector,IO.camera->orthoUp,sizeof(IO.camera->orthoUp));
 memcpy(Camera.viewPosition,IO.camera->position,sizeof(IO.camera->position));
 memcpy(Camera.viewDirection,IO.camera->viewDirection,sizeof(IO.camera->viewDirection));
 Camera.verticalFOV = IO.camera->verticalFOV;

 //read sphere
 while(IO.objects != NULL){
 if(IO.objects->type == POLYSET_OBJ){
 myTriangle *temp = new myTriangle();
 myMaterial *tempMaterial = new myMaterial();
 PolySetIO *pset = (PolySetIO *)IO.objects->data;
 PolygonIO *poly;
 VertexIO *vert;
 poly = pset->poly;
 vert = poly->vert;
 for(int i =0; i <3; i++, vert++){
 memcpy(temp->vecPositions[0],vert->pos,sizeof(vert->pos));
 }
 memcpy(temp->normal,vert->norm,sizeof(vert->norm));
 MaterialIO *material = IO.objects->material;
 memcpy(tempMaterial->ambColor,material->ambColor,sizeof(material->ambColor));
 memcpy(tempMaterial->diffColor,material->diffColor,sizeof(material->diffColor));
 memcpy(tempMaterial->emissColor,material->emissColor,sizeof(material->emissColor));
 memcpy(tempMaterial->specColor,material->specColor,sizeof(material->specColor));
 tempMaterial->ktran = material->ktran;
 tempMaterial->shininess = material->shininess;
 temp->material = tempMaterial;
 temp->next = NULL;
 if(Scene.triangles == NULL){
 Scene.triangles = temp;
 }
 else{
 Scene.triangles->next = temp;
 Scene.triangles = Scene.triangles->next;
 }
 }
 else if(IO.objects->type == SPHERE_OBJ){
 mySphere *temp = new mySphere();
 myMaterial *tempMaterial = new myMaterial();
 SphereIO *sphere = (SphereIO *)IO.objects->data;
 memcpy(temp->origin, sphere->origin, sizeof(sphere->origin));
 memcpy(temp->xaxis, sphere->xaxis, sizeof(sphere->xaxis));
 memcpy(temp->yaxis, sphere->yaxis, sizeof(sphere->yaxis));
 memcpy(temp->zaxis, sphere->zaxis, sizeof(sphere->zaxis));
 temp->radius = sphere->radius;
 temp->xlength = sphere->xlength;
 temp->ylength = sphere->ylength;
 temp->zlength = sphere->zlength;

 MaterialIO *material = IO.objects->material;
 memcpy(tempMaterial->ambColor, IO.objects->material->ambColor, sizeof(IO.objects->material->ambColor));
 memcpy(tempMaterial->diffColor, IO.objects->material->diffColor, sizeof(IO.objects->material->diffColor));
 memcpy(tempMaterial->emissColor, IO.objects->material->emissColor, sizeof(IO.objects->material->emissColor));
 memcpy(tempMaterial->specColor, IO.objects->material->specColor, sizeof(IO.objects->material->specColor));
 tempMaterial->ktran = IO.objects->material->ktran;
 tempMaterial->shininess = IO.objects->material->shininess;
 temp->next = NULL;
 temp->material = tempMaterial;
 if (Scene.spheres == NULL) {
 Scene.spheres = temp;
 }
 else {
 Scene.spheres->next = temp;
 Scene.spheres = Scene.spheres->next;
 }

 }

 IO.objects = IO.objects->next;
 }


 while(IO.lights != NULL){
 if(IO.lights->type == POINT_LIGHT){
 GVector3 lightPos;
 lightPos.x = IO.lights->position[0];
 lightPos.y = IO.lights->position[1];
 lightPos.z = IO.lights->position[2];

 GColor lightColor;
 lightColor.r = IO.lights->color[0];
 lightColor.g = IO.lights->color[1];
 lightColor.b = IO.lights->color[2];

 PointLight *templight = new PointLight(lightColor, lightPos, true);
 light

 Scene.lights = Scene.lights->next;
 }
 else if(IO.lights->type == DIRECTIONAL_LIGHT){
 myLight *temp = new myLight;
 temp->type = DIRECTIONAL_LIGHT;
 memcpy(temp->color, IO.lights->color, sizeof(IO.lights->color));
 memcpy(temp->position, IO.lights->position, sizeof(IO.lights->position));
 memcpy(temp->direction, IO.lights->direction, sizeof(IO.lights->direction));
 temp->cutOffAngle = IO.lights->cutOffAngle;
 temp->dropOffRate = IO.lights->dropOffRate;
 Scene.lights->next = temp;
 Scene.lights = Scene.lights->next;
 }
 else if(IO.lights->type == SPOT_LIGHT){
 myLight *temp = new myLight;
 temp->type = SPOT_LIGHT;
 memcpy(temp->color, IO.lights->color, sizeof(IO.lights->color));
 memcpy(temp->position, IO.lights->position, sizeof(IO.lights->position));
 memcpy(temp->direction, IO.lights->direction, sizeof(IO.lights->direction));
 temp->cutOffAngle = IO.lights->cutOffAngle;
 temp->dropOffRate = IO.lights->dropOffRate;
 Scene.lights->next = temp;
 Scene.lights = Scene.lights->next;
 }
 IO.lights = IO.lights->next;
 }
 Scene.lights = lightHead;


 }
 */
