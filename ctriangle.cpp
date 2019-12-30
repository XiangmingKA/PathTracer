//
//  ctriangle.cpp
//  CS285_Assignment2_1.0
//
//  Created by 喀相明 on 2019/5/12.
//  Copyright © 2019 喀相明. All rights reserved.
//

#include "ctriangle.h"


CTriangle:: CTriangle(){

}

CTriangle::CTriangle(GVector3 point1, GVector3 point2, GVector3 point3 ){
    Vertex1 = point1;
    Vertex2 = point2;
    Vertex3 = point3;
    max[0] = std::max(std::max(point1.x,point2.x),point3.x);
    max[1] = std::max(std::max(point1.y,point2.y),point3.y);
    max[2] = std::max(std::max(point1.z,point2.z),point3.z);
    min[0] = std::min(std::min(point1.x,point2.x),point3.x);
    min[1] = std::min(std::min(point1.y,point2.y),point3.y);
    min[2] = std::min(std::min(point1.z,point2.z),point3.z);
    if(!this->itpnorm){
        this->norm1 = (this->Vertex3-this->Vertex2).crossMul(this->Vertex1 - this->Vertex2).normalize();
        this->norm2 = this->norm1;
        this->norm3 = this->norm1;
    }
}

CTriangle::CTriangle(CTriangle& s){
    Vertex1 = s.getVertex1();
    Vertex2 = s.getVertex2();
    Vertex3 = s.getVertex3();
    max[0] = std::max(std::max(Vertex1.x,Vertex2.x),Vertex3.x);
    max[1] = std::max(std::max(Vertex1.y,Vertex2.y),Vertex3.y);
    max[2] = std::max(std::max(Vertex1.z,Vertex2.z),Vertex3.z);
    min[0] = std::min(std::min(Vertex1.x,Vertex2.x),Vertex3.x);
    min[1] = std::min(std::min(Vertex1.y,Vertex2.y),Vertex3.y);
    min[2] = std::min(std::min(Vertex1.z,Vertex2.z),Vertex3.z);
    if(!this->itpnorm){
        this->norm1 = (this->Vertex3-this->Vertex2).crossMul(this->Vertex1 - this->Vertex2).normalize();
        this->norm2 = this->norm1;
        this->norm3 = this->norm1;
    }
}

CTriangle::~CTriangle(){

}

GVector3 CTriangle::getVertex1(){
    return Vertex1;
}

GVector3 CTriangle::getVertex2(){
    return Vertex2;
}

GVector3 CTriangle::getVertex3(){
    return Vertex3;
}

void CTriangle::setPoints(GVector3& a,GVector3& b,GVector3& c){
    Vertex1 = a;
    Vertex2 = b;
    Vertex3 = c;
    max[0] = std::max(std::max(Vertex1.x,Vertex2.x),Vertex3.x);
    max[1] = std::max(std::max(Vertex1.y,Vertex2.y),Vertex3.y);
    max[2] = std::max(std::max(Vertex1.z,Vertex2.z),Vertex3.z);
    min[0] = std::min(std::min(Vertex1.x,Vertex2.x),Vertex3.x);
    min[1] = std::min(std::min(Vertex1.y,Vertex2.y),Vertex3.y);
    min[2] = std::min(std::min(Vertex1.z,Vertex2.z),Vertex3.z);
    if(!this->itpnorm){
        this->norm1 = (this->Vertex3-this->Vertex2).crossMul(this->Vertex1 - this->Vertex2).normalize();
        this->norm2 = this->norm1;
        this->norm3 = this->norm1;
    }
}

GVector3 CTriangle::getNormal(GVector3 point){
    GVector3 normal;
    if(this->itpnorm){
        //normal = this->norm1*point.x + this->norm2*point.y + this->norm3*(1-point.x-point.y);
        normal = this->norm1;
        if((1-point.x-point.y)<0)
            std::cout<<"norm wrong"<<std::endl;
        normal.normalize();
        return normal;
    }
    else{
        return this->norm1;
    }
}
/*
GVector3 CTriangle::getNormal(GVector3 point){
    GVector3 normal;

        normal = (this->Vertex3-this->Vertex2).crossMul(this->Vertex1 - this->Vertex2);

        normal.normalize();

        return normal;

}
*/
/*
IntersectResult CTriangle::isIntersected(CRay RAY){

    IntersectResult result = IntersectResult::noHit();
    GVector3 E1 ;
    GVector3 E2 ;
    GVector3 P ;

        E1 = this->Vertex2 - this->Vertex3;
        E2 = this->Vertex1 - this->Vertex3;
    P =  (RAY.getDirection()).crossMul(E2);
    float det = E1.dotMul(P);

    if(det <0.001){
        return result;
    }

    float invdet = 1/det;

    GVector3 tvec = RAY.getOrigin() - this->Vertex3;

    float u = invdet*(P.dotMul(tvec));

    if(u<0.0 || u>1.0)
        return result;

    GVector3 qvec = tvec.crossMul(E1);

    float v = invdet*(RAY.getDirection()).dotMul(qvec);

    if(v<0.0 || v +u>1.0)
        return result;

    float t = invdet*(E2.dotMul(qvec));

    if(t<0)
        return result;

        result.isHit = true;
        result.distance = t;
        result.normal = (this->getNormal(GVector3(0,0,0)));
        result.position = RAY.getPoint(t);
        //result.u = u;
        //result.v = v;

        CObject *temp = new CTriangle(this->Vertex1,this->Vertex2,this->Vertex3);
        PhongMaterial *tempM = this->phongMaterial;
        temp->phongMaterial = tempM;
        result.object = temp;

    return result;


}
*/

inline IntersectResult CTriangle::isIntersected(CRay RAY){

    IntersectResult result = IntersectResult::noHit();

    GVector3 norm = this->getNormal(GVector3(0,0,0)); // normal

    if ((norm.x==norm.y)&&(norm.z==norm.y)&&(norm.x==0.0f))  // triangle is degenerate
        return result;


    float b = norm.dotMul(RAY.getDirection());
    if (fabs(b) < 0.00001)      // ray is parallel to triangle plane
        return result;


    GVector3 w0 = RAY.getOrigin() - this->Vertex1;
    float a = -( norm.dotMul(w0) );

    float r = a / b;
    if (r < 0.0f)                 // ray goes away from triangle
        return result;                 // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect


    GVector3 interPoint = RAY.getOrigin() + RAY.getDirection() * r;


    GVector3 u = this->Vertex2 - this->Vertex1;
    GVector3 v = this->Vertex3 - this->Vertex1;

    float uu = u.dotMul(u);
    float uv = u.dotMul(v);
    float vv = v.dotMul(v);
    GVector3 w = interPoint - this->Vertex1;
    float wu = w.dotMul(u);
    float wv = w.dotMul(v);
    float D = uv * uv - uu * vv;

    // get and test parametric coords
    float s = (uv * wv - vv * wu) / D;
    if (s < 0.0f || s > 1.0f)       // I is outside T
        return result;

    float t = (uv * wu - uu * wv) / D;
    if (t < 0.0f || (s + t) > 1.0f) // I is outside T
        return result;


    GVector3 E1 ;
    GVector3 E2 ;
    GVector3 P ;
    E1 = this->Vertex2 - this->Vertex3;
    E2 = this->Vertex1 - this->Vertex3;
    P =  (RAY.getDirection()).crossMul(E2);
    float det = E1.dotMul(P);
    float invdet = 1/det;
    GVector3 tvec = RAY.getOrigin() - this->Vertex3;
    float U = invdet*(P.dotMul(tvec));
    GVector3 qvec = tvec.crossMul(E1);
    float V = invdet*(RAY.getDirection()).dotMul(qvec);
    if(U < 0 || U > 1 ||V < 0 || V> 1 || 1-U -V <0 ){
        //std::cout<<U<<","<<V<<","<<1-U-V<<std::endl;
        U = U<1 ? U: 1;
        V = V<1 ? V: 1;
        U = U>0 ? U: 0;
        V = V>0 ? V: 0;
        if((1-U-V)<0){
            U = 0;
            V = 0;
        }
    }
    result.isHit = true;
    result.distance = r;
    result.normal = norm;
    result.position = RAY.getPoint(r);
    result.u = U;
    result.v = V;

//    result.color = this->phongMaterial->sample(RAY,result.position,result.normal);

    CObject *temp = this;
    temp->phongMaterial = this->phongMaterial;
    result.object = temp;



    return result;
}


