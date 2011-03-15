/*
    Effet Ball - QPT1
    Andreas Stallinger - Programmer
    MultiMediaTechnology
    FH Salzburg
*/

#include "Vector.h"

Vector::Vector(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
};

void Vector::SetNull()
{
    x,y,z = 0;
}

void Vector::Add(float x, float y, float z)
{
    this->Add(Vector(x,y,z));
}
void Vector::Add(Vector added)
{
    this->x+=added.x;
    this->y+=added.y;
    this->z+=added.z;
}
float Vector::length()
{
    return sqrt(x*x+y*y+z*z);
}

void Vector::Normalize()
{
    float length = this->length();
    if(length)
    {
        this->x/=length;
        this->y/=length;
        this->z/=length;
    }
}
void Vector::Scale(float factor)
{
    this->x*=factor;
    this->y*=factor;
    this->z*=factor;
}

Vector Vector::cross(Vector v)
{
    return Vector(
               y * v.z - z * v.y,
               z * v.x - x * v.z,
               x * v.y - y * v.x
           );
}

void Vector::rotate(float angle, Vector ax)
{
    ax.Normalize();
    float a = (float)(angle*PI/180);
    float sina = sin( a );
    float cosa = cos( a );
    float cosb = 1.0f - cosa;

    *this = Vector( x*(ax.x*ax.x*cosb + cosa)
                    + y*(ax.x*ax.y*cosb - ax.z*sina)
                    + z*(ax.x*ax.z*cosb + ax.y*sina),
                    x*(ax.y*ax.x*cosb + ax.z*sina)
                    + y*(ax.y*ax.y*cosb + cosa)
                    + z*(ax.y*ax.z*cosb - ax.x*sina),
                    x*(ax.z*ax.x*cosb - ax.y*sina)
                    + y*(ax.z*ax.y*cosb + ax.x*sina)
                    + z*(ax.z*ax.z*cosb + cosa) );
}

Vector Vector::Difference(Vector v)
{
    return Vector(x-v.x,y-v.y,z-v.z);
}

float Vector::Distance(Vector v)
{
    Vector a = Difference(v);
    return a.length();
}

Vector Vector::operator+ (const Vector& o)
{
    return Vector((x+o.x),(y+o.y), (z+o.z));
}

Vector Vector::operator/ (const float divisor)
{
    if(divisor)
        return Vector(x/divisor,y/divisor,z/divisor);
    else
        return *this;
}

Vector Vector::operator* (const float multiplier)
{
    return Vector(x*multiplier, y*multiplier, z*multiplier);
}

void Vector::print()
{
    printf("x:%f, y:%f, z:%f ,length: %f\n", x, y,z, length());
}

static Vector add(Vector u, Vector v)
{
    return Vector(u.x + v.x, u.y + v.y, u.z + v.z);
}
static Vector sub(Vector u, Vector v)
{
    return Vector(u.x - v.x, u.y - v.y, u.z - v.z);
}
static Vector mul(Vector u, float k)
{
    return Vector(u.x * k, u.y * k, u.z * k);
}
static Vector div(Vector u, float k)
{
    return Vector(u.x / k, u.y / k, u.z / k);
}
static float dot(Vector u, Vector v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}
static float norm(Vector u)
{
    return sqrt(dot(u, u));
}
static Vector cross(Vector u, Vector v)
{
    return Vector(
               u.y * v.z - u.z * v.y,
               u.z * v.x - u.x * v.z,
               u.x * v.y - u.y * v.x
           );
}
