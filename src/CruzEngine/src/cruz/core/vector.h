#pragma once
#include <iostream>

class Vector3 {
public:
    float x, y, z;

    Vector3();
    Vector3(float x, float y, float z);

    Vector3 operator+(const Vector3& v) const;
    Vector3 operator-(const Vector3& v) const;
    Vector3 operator*(float scalar) const;

    float length() const;
    Vector3 normalized() const;
    void print() const;
};

class Vector2 {
public:
    float x, y;

    Vector2();
    Vector2(float x, float y);

    Vector2 operator+(const Vector2& v) const;
    Vector2 operator-(const Vector2& v) const;
    Vector2 operator*(float scalar) const;
    Vector2 operator/(float scalar) const;

    Vector2& operator+=(const Vector2& v);
    Vector2& operator-=(const Vector2& v);
    Vector2& operator*=(float scalar);
    Vector2& operator/=(float scalar);

    float length() const;
    Vector2 normalized() const;
    void print() const;
};