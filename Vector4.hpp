#pragma once

extern const double wZero;

class Vector4
{
    
public:
    float x, y, z,w;
    Vector4():x(0), y(0), z(0),w(0){}
    Vector4(float x1, float y1, float z1, float w1):x(x1), y(y1), z(z1),w(w1){}
    Vector4(const Vector4 &v);
    ~Vector4();
    Vector4 &operator=(const Vector4 &v);
    Vector4 operator+(const Vector4 &v);
    Vector4 operator-(const Vector4 &v);
    Vector4 operator/(const Vector4 &v);
    Vector4 operator*(const Vector4 &v);
    Vector4 operator+(float f);
    Vector4 operator-(float f);
    Vector4 operator/(float f);
    Vector4 operator*(float f);
    float dot(const Vector4 &v);
    float length();
    void normalize();
    Vector4 crossProduct(const Vector4 &v);
    void printVec4();
};
