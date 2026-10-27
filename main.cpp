
#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittables_list.h"
#include "sphere.h"

// double hit_sphere(const point3& center, double radius, const ray& r) {
//     vec3 oc = center - r.origin(); // C-Q, C is the spheres center and Q is the vector pointing from the camera to the spheres center
//     // auto a = dot(r.direction(), r.direction());
//     // auto b = -2.0 * dot (r.direction(), oc);
//     // auto c = dot(oc, oc) - radius*radius;
//     // auto discriminant = b*b - 4*a*c;

//     auto a = r.direction().length_squared();
//     auto h = dot(r.direction(), oc);
//     auto c = oc.length_squared() - radius * radius;
//     auto discriminant = h * h - a * c;


//     if (discriminant < 0)
//     { // if discriminant < 0 then the ray does not hit the sphere, no intersection, zero means it grazes the surface,
//         // positive means it passes through two points
//         return -1.0;
//     }
//     else { // since t is positive from quadratic formula we return the t value that is the closest one to us, the first interesction
//         //the first t (lower value) is the value when we first hit the sphere's surface, then the second t (higher value) is the value
//         //when we hit the other end of the sphere
//         //(-b - sqrt(b^2 -4ac)) /  2.0*a is the lesser value since we use the '-' after -b
//         return (h-std::sqrt(discriminant)) / a;
//     }
// } 

int main() {
    
    hittable_list world;

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 400;
    cam.samples_per_pixel = 100;

    cam.render(world);
}