/*
    Effet Ball - QPT1
    Andreas Stallinger - Programmer
    MultiMediaTechnology
    FH Salzburg
*/

#ifndef EFFETBALLVECTOR_H_INCLUDED
#define EFFETBALLVECTOR_H_INCLUDED

#include <cstdio>
#include <cmath>
#include "ofMain.h"

//#define PI (2*acos())

class Vector
{
public:
    float x;
    float y;
    float z;

    Vector(float x=0, float y=0, float z=0);
    //reset vector (0,0,0)
    void SetNull();
    //addition to the vector
    void Add(float x=0, float y=0, float z=0);
    void Add(Vector added);

    Vector operator+ (const Vector& o);
    Vector operator/ (const float divisor);
    Vector operator* (const float multiplier);

    float length();
    void Normalize();
    void Scale(float);
    void rotate(float angle, Vector axis);
    Vector cross(Vector v);

    Vector Difference(Vector);
    float Distance(Vector);

    void print();

    static Vector add(Vector u, Vector v);
    static Vector sub(Vector u, Vector v);
    static Vector mul(Vector u, float k);
    static Vector div(Vector u, float k);
    static float dot(Vector u, Vector v);
    static float norm(Vector u);
    static Vector cross(Vector u, Vector v);

};

#endif
