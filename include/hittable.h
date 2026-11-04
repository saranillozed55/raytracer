#ifndef HITTABLE_H
#define HITTABLE_H

// #include "ray.h"
// #include "vec3.h"
// #include "point3.h"
// #include "interval.h"
// #include <memory>

class material;

//hit_record is a way to stuff a bunch of arguments into a class we can pass it around as a group
//This class is used to store information about a ray-object intersection
class hit_record {
    public:
        point3 p;
        vec3 normal;
        shared_ptr<material> mat;
        double t;
        bool front_face;

        void set_face_normal(const ray&r, const vec3& outward_normal) {
            //sets the hit record normal vector
            //NOTE: the parameter 'outward_normal" is assumed to have unit_length
            // for better visualization of how this works, in screenshots(Fedora)

            // if the front_face is < 0 then the angle between the N and R is an obtuse angle, since its obtuse cos(theta) is negative, indicating ray is outside
            // if the front_face is > 0 then angle between the Normal and Ray is acute so its positive, indicating its inside the sphere

            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
            
        }


};

class hittable {
    public:
        virtual ~hittable() = default;

        //hittable::hit using interval
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0; 
        
};

#endif