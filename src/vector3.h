//==============================================================================
// James McCormick - vector3.h
// Description 
// 		A class to handle Vector math. A 3 valued vector.
//==============================================================================

#ifndef _VECTOR3_
#define _VECTOR3_

#include <cmath>


#define Zero_Tolerance 0.0001


//------------------------------------------------------------------------------
// Vector class -- A 3D vector data structure and vector operations
//------------------------------------------------------------------------------
template<typename real>
class vector3
{
public:
    real x, y, z;

    // constructors
    vector3(const real& i = 0, const real& j = 0, const real& k = 0)
    {
        x = i;
        y = j;
        z = k;
    }

    // functions
    real magnitude() const
    {
        return sqrt((real)(x*x + y*y + z*z));
    }

    void reverse()
    {
        x = -x;
        y = -y;
        z = -z;
    }

    void normalize()
    {
        real c = magnitude();
        if(c <= Zero_Tolerance) c = 1.0;
        x /= c;
        y /= c;
        z /= c;

        if(fabs(x) < Zero_Tolerance) x = 0;
        if(fabs(y) < Zero_Tolerance) y = 0;
        if(fabs(z) < Zero_Tolerance) z = 0;
    }

    vector3<real> normalVector() const
    {
        real a(x), b(y), c(z), d = magnitude();
        if(d <= Zero_Tolerance) d = 1.0;
        a /= d;
        b /= d;
        c /= d;

        if(fabs(a) < Zero_Tolerance) a = 0;
        if(fabs(b) < Zero_Tolerance) b = 0;
        if(fabs(c) < Zero_Tolerance) c = 0;

        return vector3<real>(a, b, c);
    }

    vector3<real> crossProduct(const vector3<real>& u) const
    {
        return vector3<real>(y*u.z - z*u.y, -x*u.z + z*u.x, x*u.y - y*u.x);
    }

    // Accessors
    real& X() { return x; }
    real X() const { return x; }

    real& Y() { return y; }
    real Y() const { return y; }

    real& Z() { return z; }
    real Z() const { return z; }

    // comparison
    const bool operator== (const vector3<real>& t) const
    {
        if(x != t.x) return false;
        if(y != t.y) return false;
        if(z != t.z) return false;
        return true;
    }

    const bool operator!= (const vector3<real>& t) const
    {
        if(x != t.x) return true;
        if(y != t.y) return true;
        if(z != t.z) return true;
        return false;
    }

    // assignment
    vector3<real>& operator= (const vector3<real>& t)
    {
        x = t.x;
        y = t.y;
        z = t.z;
        return *this;
    }

    // operators
    vector3<real>& operator+=(const vector3<real>& t)
    {
        x += t.x;
        y += t.y;
        z += t.z;
        return *this;
    }

    vector3<real>& operator-=(const vector3<real>& t)
    {
        x -= t.x;
        y -= t.y;
        z -= t.z;
        return *this;
    }

    vector3<real>& operator*=(const real& r)
    {
        x *= r;
        y *= r;
        z *= r;
        return *this;
    }

    vector3<real>& operator/=(const real& r)
    {
        x /= r;
        y /= r;
        z /= r;
        return *this;
    }

    // unary minus
    const vector3<real> operator-() const
    {
        return vector3<real>(-x, -y, -z);
    }

    // Friends

    //------------------------------------------------------------------------------
    // addition operator: adds u to t => u + t
    //------------------------------------------------------------------------------
    friend vector3<real> operator+(const vector3<real>& u, const vector3<real>& t)
    {
        return vector3<real>(u.x + t.x, u.y + t.y, u.z + t.z);
    }

    //------------------------------------------------------------------------------
    // subtraction operator: subtracts t from u => u - t
    //------------------------------------------------------------------------------
    friend vector3<real> operator-(const vector3<real>& u, const vector3<real>& t)
    {
        return vector3<real>(u.x - t.x, u.y - t.y, u.z - t.z);
    }

    //------------------------------------------------------------------------------
    // performs a scalar multiplication to vector u
    //------------------------------------------------------------------------------
    friend vector3<real> operator*(const real& r, const vector3<real>& u)
    {
        return vector3<real>(u.x*r, u.y*r, u.z*r);
    }

    friend vector3<real> operator*(const vector3<real>& u, const real& r)
    {
        return vector3<real>(u.x*r, u.y*r, u.z*r);
    }

    //------------------------------------------------------------------------------
    // performs a dot product between u and t
    //------------------------------------------------------------------------------
    friend real operator*(const vector3<real>& u, const vector3<real>& t)
    {
        return (u.x * t.x + u.y * t.y + u.z * t.z);
    }

    //------------------------------------------------------------------------------
    // performs a scalar division on vector u
    //------------------------------------------------------------------------------
    friend vector3<real> operator/(const vector3<real>& u, const real& r)
    {
        return vector3<real>(u.x/r, u.y/r, u.z/r);
    }
};

typedef vector3<float> vector3f;
typedef vector3<double> vector3d;

#endif
