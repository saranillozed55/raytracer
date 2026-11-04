#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:
        ray() {}

        ray(const point3& origin, const vec3& direction, double time) : orig(origin), dir(direction), tm(time) {}

        ray(const point3& origin, const vec3& direction) : ray(origin, direction, 0.0) {} // Uses the three-argument constructor with a default time of 0.0

        const point3& origin() const {return orig;}
        const vec3& direction() const {return dir;}
        double time() const {return tm;}

        //where the point is on the ray P = A +tb, where A is the origin, t is any real number, and b is the direction
        point3 at(double t) const {
            return orig + t*dir;
        }

    private:
        point3 orig;
        vec3 dir;
        double tm;
};

#endif