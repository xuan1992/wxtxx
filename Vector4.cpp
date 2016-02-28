//
//  Vector4.cpp
//  TEST
//
//  Created by WU XUAN on 16/2/1.
//  Copyright © 2016年 WU XUAN. All rights reserved.
//

#include "Vector4.hpp"

//#include"Plane.h"
#include<iostream>
#include<math.h>

using namespace std;

const double wZero = 1e-6;

//复制构造函数，必须为常量引用参数，否则编译不通过
Vector4::Vector4(const Vector4 &v):x(v.x), y(v.y), z(v.z), w(v.w)
{
}

Vector4::~Vector4()
{
}

Vector4 &Vector4::operator=(const Vector4 &v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
}

Vector4 Vector4::operator+(const Vector4 &v)
{
    return Vector4(x+v.x, y+v.y, z+v.z, w+v.w);
}

Vector4 Vector4::operator-(const Vector4 &v)
{
    return Vector4(x-v.x, y-v.y, z-v.z, w-v.w);
}

Vector4 Vector4::operator/(const Vector4 &v)
{
    if (fabsf(v.x) <= wZero || fabsf(v.y) <= wZero || fabsf(v.z) <= wZero || fabsf(v.w) <= wZero)
    {
        std::cerr<<"Over flow!\n";
        return *this;
    }
    return Vector4(x/v.x, y/v.y, z/v.z, w/v.w);
}

Vector4 Vector4::operator*(const Vector4 &v)
{
    return Vector4(x*v.x, y*v.y, z*v.z, w*v.w);
}

Vector4 Vector4::operator+(float f)
{
    return Vector4(x+f, y+f, z+f, w+f);
}

Vector4 Vector4::operator-(float f)
{
    return Vector4(x-f, y-f, z-f, w-f);
}

Vector4 Vector4::operator/(float f)
{
    if (fabsf(f) < wZero)
    {
        std::cerr<<"Over flow!\n";
        return *this;
    }
    return Vector4(x/f, y/f, z/f, w/f);
}

Vector4 Vector4::operator*(float f)
{
    return Vector4(x*f, y*f, z*f, w*f);
}

float Vector4::dot(const Vector4 &v)
{
    return x*v.x + y*v.y + z*v.z + w*v.w;
}

float Vector4::length()
{
    return sqrtf(dot(*this));
}

void Vector4::normalize()
{
    float len = length();
    if (len < wZero) len = 1;
    len = 1/len;
    
    x *= len;
    y *= len;
    z *= len;
    w *= len;
}

/*
 Cross Product叉乘公式
 aXb = | i,  j,  k  |
 | a.x a.y a.z|
 | b.x b.y b.z| = (a.x*b.z -a.z*b.y)i + (a.z*b.x - a.x*b.z)j + (a.x+b.y - a.y*b.x)k
 */
//Vector4 Vector4::crossProduct(const Vector4 &v)
//{
//    return Vector4(y * v.z - z * v.y,
//                   z * v.x - x * v.z,
//                   x * v.y - y * v.x);
//}

void Vector4::printVec4()
{
    std::cout<<"("<<x<<", "<<y<<", "<<z<<", "<<w<<")"<<std::endl;
}