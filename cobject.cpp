//
//  cobject.cpp
//  CS285_Assignment2_1.0
//
//  Created by 喀相明 on 2019/5/12.
//  Copyright © 2019 喀相明. All rights reserved.
//

#include <stdio.h>
#include "cobject.h"
#include "cray.h"
#include "global.h"
#include "boundingbox.h"

int numOfMaxAABBs = 3;


CObject:: CObject(){

}

CObject:: ~CObject(){

}

IntersectResult CObject::isIntersected(CRay _ray){
    IntersectResult res;
    res.isHit = false;
    return res;
}

bool CObject::bounding_box(float t0, float t1, aabb& box){
    return false;
}

bool CObject::hit(CRay ray, float t_min, float t_max, IntersectResult &result){
    return false;
}

Union::Union()
{
    //ctor
}

Union::~Union()
{
    //dtor
}
void Union::push(CObject* object)

{
    cobjects.push_back(object);
}
inline IntersectResult Union::isIntersected(CRay _ray)
{
    std::cout<<"loop"<<std::endl;
    const float Infinity=1e30;
    float minDistance = Infinity;
    IntersectResult minResult = IntersectResult::noHit();
    long size=this->cobjects.size();
    for (long i=0;i<size;++i){
        IntersectResult result = this->cobjects[i]->isIntersected(_ray);
        if (result.isHit && (result.distance < minDistance)) {
            minDistance = result.distance;
            minResult = result;
            minResult.isHit =true;


            minResult.distance = result.distance;
            minResult.normal = result.normal;
            minResult.position = result.position;
            minResult = result;
        }
    }
    return minResult;
}

myBvhNode::myBvhNode(){}

myBvhNode::~myBvhNode(){}
//myBvhNode::~myBvhNode(){}


vector<aabb*> aabblist;

myBvhNode* headNode = new myBvhNode();

//myBvhNode* headNode = new myBvhNode();

void constructAABBBox(Union objects){
    long size= objects.cobjects.size();
    for (long i=0;i<size;++i){
        aabb* temp = new aabb(objects.cobjects[i]->min, objects.cobjects[i]->max, objects.cobjects[i]);
        aabblist.push_back(temp);
    }


}


void constructBvhTree(myBvhNode& head,vector<aabb*> aabbs,int level ){
    int numOfAABB = aabbs.size();
    float min[3] = {100,100,100};
    float max[3] = {-100,-100,-100};
    for(int i =0; i< numOfAABB ; i++){
        for(int j =0 ; j<3 ; j++){
            min[j] = min[j] > aabbs[i]->min[j] ? aabbs[i]->min[j] : min[j];
            max[j] = max[j] < aabbs[i]->max[j] ? aabbs[i]->max[j] : max[j];
        }
    }

    if(numOfAABB>30){
        std::cout<<"Build Node: "<<level<<" Num: "<<numOfAABB<<std::endl;
        std::cout<<"Max:"<<max[0]<<","<<max[1]<<","<<max[2]<<std::endl;
        std::cout<<"Min:"<<min[0]<<","<<min[1]<<","<<min[2]<<std::endl;
        float lx = max[0] - min[0];
        float ly = max[1] - min[1];
        float lz = max[2] - min[2];
        vector<aabb*> leftlist;
        vector<aabb*> rightlist;
        if(lx > ly && lx >lz)
        {
            std::cout<<"x"<<std::endl;
            for(int j =0; j<numOfAABB-1 ;j++){
                //j
                for(int i =0; i<=j ;i++){
                    if((aabbs[i]->max[0]+aabbs[i]->min[0])>(aabbs[i+1]->max[0]+aabbs[i+1]->min[0])){
                        aabb* tempPtr = aabbs[i+1];
                        aabbs[i+1] = aabbs[i];
                        aabbs[i] = tempPtr;
                    }
                }
            }

            if(numOfAABB%2==0){
                for (int i = 0; i< numOfAABB/2; i++) {
                    leftlist.push_back(aabbs[i]);
                    rightlist.push_back(aabbs[numOfAABB-i-1]);
                }
            }
            else{
                for (int i = 0; i< numOfAABB/2; i++) {
                    leftlist.push_back(aabbs[i]);
                    rightlist.push_back(aabbs[numOfAABB-i-1]);
                }
                leftlist.push_back(aabbs[numOfAABB/2+1]);
            }
        }
        else if(ly > lx && ly >lz)
        {
            std::cout<<"y"<<std::endl;
            for(int j =0; j<numOfAABB-1 ;j++){
                for(int i =0; i<=j ;i++){
                    if((aabbs[i]->max[1]+aabbs[i]->min[1])>(aabbs[i+1]->max[1]+aabbs[i+1]->min[1])){
                        aabb* tempPtr = aabbs[i+1];
                        aabbs[i+1] = aabbs[i];
                        aabbs[i] = tempPtr;
                    }
                }
            }

            if(numOfAABB%2==0){
                for (int i = 0; i< numOfAABB/2; i++) {
                    leftlist.push_back(aabbs[i]);
                    rightlist.push_back(aabbs[numOfAABB-i-1]);
                }
            }
            else{
                for (int i = 0; i< numOfAABB/2; i++) {
                    leftlist.push_back(aabbs[i]);
                    rightlist.push_back(aabbs[numOfAABB-i-1]);
                }
                leftlist.push_back(aabbs[numOfAABB/2+1]);
            }
        }
        else{
            std::cout<<"z"<<std::endl;
            for(int j =0; j<numOfAABB-1 ;j++){
                for(int i =0; i<=j ;i++){
                    if((aabbs[i]->max[2]+aabbs[i]->min[2])>(aabbs[i+1]->max[2]+aabbs[i+1]->min[2])){
                        aabb* tempPtr = aabbs[i+1];
                        aabbs[i+1] = aabbs[i];
                        aabbs[i] = tempPtr;
                    }
                }
            }

            if(numOfAABB%2==0){
                for (int i = 0; i< numOfAABB/2; i++) {
                    leftlist.push_back(aabbs[i]);
                    rightlist.push_back(aabbs[numOfAABB-i-1]);
                }
            }
            else{
                for (int i = 0; i< numOfAABB/2; i++) {
                    leftlist.push_back(aabbs[i]);
                    rightlist.push_back(aabbs[numOfAABB-i-1]);
                }
                leftlist.push_back(aabbs[numOfAABB/2+1]);
            }
        }
        head.isLeaf = false;
        for(int i =0; i<3;i++){
            head.max[i] = max[i];
            head.min[i] = min[i];
        }
        myBvhNode *leftNode = new myBvhNode();
        myBvhNode *rightNode = new myBvhNode();
        head.left = leftNode;
        head.right=rightNode;

        constructBvhTree(*head.left,leftlist,level+1);
        constructBvhTree(*head.right,rightlist,level+1);
    }
    else
    {
        head.isLeaf = true;
        for(int i =0; i<3;i++){
            head.max[i] = max[i];
            head.min[i] = min[i];
        }
        head.aabbBoxes = aabbs;
        std::cout<<"Build Leaf: "<<level<<" Num: "<<numOfAABB<<std::endl;
        std::cout<<"Max:"<<max[0]<<","<<max[1]<<","<<max[2]<<std::endl;
        std::cout<<"Min:"<<min[0]<<","<<min[1]<<","<<min[2]<<std::endl;
    }
}

void constructBvhTree2(myBvhNode& head,vector<aabb*> aabbs,int level ){
    int numOfAABB = aabbs.size();
    float min[3] = {1000,1000,1000};
    float max[3] = {-1000,-1000,-1000};
    for(int i =0; i< numOfAABB ; i++){
        for(int j =0 ; j<3 ; j++){
            min[j] = min[j] > aabbs[i]->min[j] ? aabbs[i]->min[j] : min[j];
            max[j] = max[j] < aabbs[i]->max[j] ? aabbs[i]->max[j] : max[j];
        }
    }

    if(numOfAABB>4){
        std::cout<<"Build Node: "<<level<<" Num: "<<numOfAABB<<std::endl;
        std::cout<<"Max:"<<max[0]<<","<<max[1]<<","<<max[2]<<std::endl;
        std::cout<<"Min:"<<min[0]<<","<<min[1]<<","<<min[2]<<std::endl;
        float lx = max[0] - min[0];
        float ly = max[1] - min[1];
        float lz = max[2] - min[2];
        vector<aabb*> leftlist;
        vector<aabb*> rightlist;
        if(lx > ly && lx >lz)
        {
            std::cout<<"x"<<std::endl;
            std::sort(aabbs.begin(), aabbs.end(), [](auto f1, auto f2) {return f1->max[0] + f1->min[0]< f2->max[0] + f2->min[0];});
            auto beginning = aabbs.begin();
            auto middling  = aabbs.begin() + (aabbs.size() / 2);
            auto ending    = aabbs.end();

            leftlist  = std::vector<aabb*>(beginning, middling);
            rightlist = std::vector<aabb*>(middling, ending);
        }
        else if(ly > lx && ly >lz)
        {

            std::cout<<"y"<<std::endl;
            std::sort(aabbs.begin(), aabbs.end(), [](auto f1, auto f2) {return f1->max[1] + f1->min[1]< f2->max[1] + f2->min[1];});

            auto beginning = aabbs.begin();
            auto middling  = aabbs.begin() + (aabbs.size() / 2);
            auto ending    = aabbs.end();

            leftlist  = std::vector<aabb*>(beginning, middling);
            rightlist = std::vector<aabb*>(middling, ending);
        }
        else{
            std::cout<<"z"<<std::endl;

            std::sort(aabbs.begin(), aabbs.end(), [](auto f1, auto f2) {return f1->max[2] + f1->min[2]< f2->max[2] + f2->min[2];});

            auto beginning = aabbs.begin();
            auto middling  = aabbs.begin() + (aabbs.size() / 2);
            auto ending    = aabbs.end();

            leftlist  = std::vector<aabb*>(beginning, middling);
            rightlist = std::vector<aabb*>(middling, ending);
        }
        head.isLeaf = false;
        for(int i =0; i<3;i++){
            head.max[i] = max[i];
            head.min[i] = min[i];
        }
        myBvhNode *leftNode = new myBvhNode();
        myBvhNode *rightNode = new myBvhNode();
        head.left = leftNode;
        head.right=rightNode;

        constructBvhTree2(*head.left,leftlist,level+1);
        constructBvhTree2(*head.right,rightlist,level+1);
    }
    else
    {
        head.isLeaf = true;
        for(int i =0; i<3;i++){
            head.max[i] = max[i];
            head.min[i] = min[i];
        }
        head.aabbBoxes = aabbs;
        std::cout<<"Build Leaf: "<<level<<" Num: "<<numOfAABB<<std::endl;
        std::cout<<"Max:"<<max[0]<<","<<max[1]<<","<<max[2]<<std::endl;
        std::cout<<"Min:"<<min[0]<<","<<min[1]<<","<<min[2]<<std::endl;
    }
}

bool hitBVH(myBvhNode* head, CRay Ray){
    if(head->isLeaf==false){
        float rayDir[3];
        rayDir[0] = Ray.getDirection().x;
        rayDir[1] = Ray.getDirection().y;
        rayDir[2] = Ray.getDirection().z;
        float rayOri[3];
        rayOri[0] = Ray.getOrigin().x;
        rayOri[1] = Ray.getOrigin().y;
        rayOri[2] = Ray.getOrigin().z;

        float tmin=-10,tmax=10;//0,0

        for(int i =0; i <3; i++){

            float invD = 1.0f / rayDir[i];
            float t0 = (head->min[i] - rayOri[i])*invD;
            float t1 = (head->max[i] - rayOri[i])*invD;
            if(invD < 0.0f)
                std::swap(t0, t1);
            tmin = t0 < tmin ? t0 : tmin;
            tmax = t1 > tmax ? t1 : tmax;
            if(tmax <= tmin)
                return false;
        }

        return (hitBVH(head->left, Ray)||hitBVH(head->right, Ray));
    }
    else{
        float rayDir[3];
        rayDir[0] = Ray.getDirection().x;
        rayDir[1] = Ray.getDirection().y;
        rayDir[2] = Ray.getDirection().z;
        float rayOri[3];
        rayOri[0] = Ray.getOrigin().x;
        rayOri[1] = Ray.getOrigin().y;
        rayOri[2] = Ray.getOrigin().z;

        float tmin=-10,tmax=10;

        for(int i =0; i <3; i++){

            float invD = 1.0f / rayDir[i];
            float t0 = (head->min[i] - rayOri[i])*invD;
            float t1 = (head->max[i] - rayOri[i])*invD;
            if(invD < 0.0f)
                std::swap(t0, t1);
            tmin = t0 < tmin ? t0 : tmin;
            tmax = t1 > tmax ? t1 : tmax;
            if(tmax <= tmin)
                return false;
        }
        bool flag = false;
        int size = head->aabbBoxes.size();
        for(int i =0;i<size;i++){
            if(head->aabbBoxes[i]->hit(Ray, -10, 10))
                flag = true;

        }
        return flag;
    }
}

void buildBVHtree(){
    constructAABBBox(objects);
    constructBvhTree2(*headNode,aabblist,0);

}

double ffmax(double a , double b){
    return a > b ? a : b;
}

double ffmin(double a , double b){
    return a < b ? a : b;
}

inline bool AABBintersect(float max[3], float min[3], CRay Ray){

    double tx_min,ty_min,tz_min;
    double tx_max,ty_max,tz_max;
    float lightDir[3] = {(Ray.getDirection()).x,(Ray.getDirection()).y,(Ray.getDirection()).z};
    float lightOrg[3] ={(Ray.getOrigin()).x,(Ray.getOrigin()).y,(Ray.getOrigin()).z};

    if(abs(lightDir[0]) < 0.000001f)
    {

        if(lightOrg[0] < max[0] || lightOrg[0] > min[0])
            return false ;
    }
    else
    {
        if(lightDir[0]>=0)
        {
            tx_min = (min[0]-(lightOrg[0]))/lightDir[0];
            tx_max = (max[0]-(lightOrg[0]))/lightDir[0];
        }
        else
        {
            tx_min = (max[0]-(lightOrg[0]))/lightDir[0];
            tx_max = (min[0]-(lightOrg[0]))/lightDir[0];
        }

    }


    if(abs(lightDir[1]) < 0.000001f)
    {

        if(lightOrg[1] < max[1] || lightOrg[1] > min[1])
            return false ;
    }
    else
    {
        if((Ray.getDirection()).y>=0)
        {
            ty_min = (min[1]-(lightOrg[1]))/(Ray.getDirection()).y;
            ty_max = (max[1]-(lightOrg[1]))/(Ray.getDirection()).y;
        }
        else
        {
            ty_min = (max[1]-(Ray.getOrigin()).y)/(Ray.getDirection()).y;
            ty_max = (min[1]-(Ray.getOrigin()).y)/(Ray.getDirection()).y;
        }

    }


    if(abs(lightDir[2]) < 0.000001f)
    {

        if((Ray.getOrigin()).z < max[2] || (Ray.getOrigin()).z > min[2])
            return false ;
    }
    else
    {
        if((Ray.getDirection()).z>=0)
        {
            tz_min = (min[2]-(Ray.getOrigin()).z)/(Ray.getDirection()).z;
            tz_max = (max[2]-(Ray.getOrigin()).z)/(Ray.getDirection()).z;
        }
        else
        {
            tz_min = (max[2]-(Ray.getOrigin()).z)/(Ray.getDirection()).z;
            tz_max = (min[2]-(Ray.getOrigin()).z)/(Ray.getDirection()).z;
        }

    }

    double t0,t1;


    t0=std::max(tz_min,std::max(tx_min,ty_min));


    t1=std::min(tz_max,std::min(tx_max,ty_max));

    return t0<= t1;
}

IntersectResult intersectBVH(myBvhNode* head , CRay Ray ){
    IntersectResult result;
    IntersectResult resultleft ;
    IntersectResult resultright;
    resultleft.isHit=false;
    resultright.isHit=false;
    result.isHit=false;

    if(!AABBintersect(head->max, head->min, Ray)){
        //std::cout<<"Miss"<<std::endl;
        return result;
    }

    if(head->isLeaf==false){


        resultleft = intersectBVH(head->left, Ray);
        resultright = intersectBVH(head->right, Ray);
        if(!(resultright.isHit||resultleft.isHit)){
            return result;
        }
        else if(resultright.isHit&&resultleft.isHit){
            if(resultright.distance>resultleft.distance){
                return resultleft;
            }
            else{
                return resultright;
            }
        }
        else if(resultright.isHit){
            return resultright;
        }
        else{
            return resultleft;
        }
    }
    else{

        IntersectResult tempres;
        float dis = 1000;
        int size = head->aabbBoxes.size();
        for(int i =0;i<size;i++){
            if(AABBintersect(head->aabbBoxes[i]->max, head->aabbBoxes[i]->min ,Ray)){//head->aabbBoxes[i]->hit(Ray, 0, 10)
                tempres= head->aabbBoxes[i]->object->isIntersected(Ray);
                //head: 只有物体没有背景
                //1:物体颜色缺失
                if(tempres.isHit&&(tempres.distance<dis+0.001 && tempres.distance>0)){
                    //std::cout<<"Hit"<<std::endl;
                    dis = tempres.distance;
                    result = tempres;
                }
            }
        }
        return result;
    }

}

std::vector<GColor> myTexture1;


CPlane::CPlane(){};
CPlane::~CPlane(){};
CPlane::CPlane(GVector3 norm, float dis){
    this->norm = norm;
    this->distance = dis;
}

IntersectResult CPlane::isIntersected(CRay ray){
    IntersectResult result = IntersectResult::noHit();

    float a = -(ray.getDirection().dotMul(this->norm));
    if (1)
    {
        result.isHit=1;
        result.object = this;
        float b = this->norm.dotMul(ray.getOrigin()-(camera.eye+norm*-distance));
        result.distance = abs(b / a);
        result.position = ray.getPoint(result.distance);
        result.normal = (this->norm).negate();
        return result;
    }
    return result;
}

