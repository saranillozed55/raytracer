#ifndef SPHERE_H
#define SPHERE_H
#include "hittable.h"

class sphere : public hittable {
    public:
        sphere(const point3& center, double radius) : center(center), radius(std::fmax(0,radius)) {}

        bool hit(const ray& r, interval ray_t,  hit_record& rec) const override {
            vec3 oc = center - r.origin(); // C-Q, C is the spheres center and Q is the vector pointing from the camera to the spheres center
            // auto a = dot(r.direction(), r.direction());
            // auto b = -2.0 * dot (r.direction(), oc);
            // auto c = dot(oc, oc) - radius*radius;
            // auto discriminant = b*b - 4*a*c;

            auto a = r.direction().length_squared();
            auto h = dot(r.direction() ,oc);
            auto c = oc.length_squared() - radius* radius;
            auto discriminant = h*h - a*c;

            if(discriminant < 0) { // if discriminant < 0 then the ray does not hit the sphere, no intersection, zero means it grazes the surface,
                //positive means it passes through two points
                return false;
            }

            auto sqrtd = std::sqrt(discriminant);

            //find the nearest root that lies in the acceptable range
            //sphere using interval
            auto root = (h-sqrtd) / a;
            if(!ray_t.surrounds(root)) {
                root = (h+sqrtd) / a;
                if(!ray_t.surrounds(root)) return false;
            }
            rec.t = root;
            rec.p = r.at(rec.t);


            //normal vector must be a unit vector so because the equation for the outward normal is P-C
            //Point - Center is the outward normal, Center - Point is the inward normal
            //Using Point - Center, we get the unit vector by by dividing it by the radius since the radius is just the distance
            // between the two points ||P-C|| = Radius
            // (P-C) / ||P-C||
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal); //set the front_face, hittable.h checks if its outside or inside
            


            return true;
        }

    private:
        point3 center;
        double radius;
};

#endif