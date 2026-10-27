#ifndef VEC3_H
#define VEC3_H

#include "rtweekend.h"

class vec3 {
    public: 
        double e[3];
        vec3() : e{0,0,0} 
        {

        } // this is a default constructor
        vec3(double e0, double e1, double e2) : e{e0,e1,e2} 
        {

        } // this is a constructor to directly initialize the values of the double array

        //const after here for the function tells the compiler we promise that we don't change the 
        //value of point for x,y, or z. So we only read.
        double x() const {return e[0];}
        double y() const {return e[1];}
        double z() const {return e[2];}

        vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}
        double operator[] (int i) const {return e[i];}
        double& operator[] (int i) {return e[i];}


        //
        vec3& operator+= (const vec3& v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];

            return *this;
        }

        vec3& operator*= (double t) {
            e[0] *=t;
            e[1] *=t;
            e[2] *=t;  
            return *this;  
        }

        vec3& operator/= (double t) {
            return *this *= 1/t;
        }

        double length() const {
            return std::sqrt(length_squared());
        }

        double length_squared() const {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }

        static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }

        static vec3 random(double min, double max) {
            return vec3(random_double(min, max),random_double(min, max), random_double(min, max));
        }

        //Return true if the vector is close to zero in all dimensions
        bool near_zero() const {
            auto s = 1e-8;

            //fabs is returns absolute vlaue of a floating point number
            return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s)  && (std::fabs(e[2]) < s);
        }
};
//point 3 is an alias or vec, but useful for geometric clarity in the code
using point3 = vec3;

//vector utility functions 

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}
inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
inline vec3 operator*(double t, const vec3& v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}
inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}
inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}
inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}
inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]
    );
}
inline vec3 unit_vector(const vec3& v) {
    return v/v.length();
}

inline vec3 random_unit_vector() {
    while(true) {
        auto p = vec3::random(-1,1);
        auto lensq = p.length_squared();

        //floating point abstraction leak because floating point values have finite precision
        //if all three coordinates are small enough the norm of the vector could be 0 thus giving us a normalized vector of
        // [+infinity, + infinity, +infinity]
        if(1e-160 < lensq && lensq <= 1) {
            return p/sqrt(lensq);
        }
    }
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if(dot(on_unit_sphere, normal) > 0.0) { // in the same hemisphere as the normal
        return on_unit_sphere;
    }
    else {
        return -on_unit_sphere;
    }
}

#endif