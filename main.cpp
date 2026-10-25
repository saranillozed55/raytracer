#include <iostream>
#include "vec3.h"
#include "color.h"
#include "ray.h"

//6.4 was where i left off
double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = center - r.origin(); // C-Q, C is the spheres center and Q is the vector pointing from the camera to the spheres center
    // auto a = dot(r.direction(), r.direction());
    // auto b = -2.0 * dot (r.direction(), oc);
    // auto c = dot(oc, oc) - radius*radius;
    // auto discriminant = b*b - 4*a*c;

    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = h * h - a * c;


    if (discriminant < 0)
    { // if discriminant < 0 then the ray does not hit the sphere, no intersection, zero means it grazes the surface,
        // positive means it passes through two points
        return -1.0;
    }
    else { // since t is positive from quadratic formula we return the t value that is the closest one to us, the first interesction
        //the first t (lower value) is the value when we first hit the sphere's surface, then the second t (higher value) is the value
        //when we hit the other end of the sphere
        //(-b - sqrt(b^2 -4ac)) /  2.0*a is the lesser value since we use the '-' after -b
        return (h-std::sqrt(discriminant)) / a;
    }
}

color ray_color(const ray &r)
{

    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);

    if (t > 0.0)
    {                                                 // hit the surface of the sphere
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1)); // P - C is the normal of the surface at that point, vector from the sphere's center to the hit point

        //N is a unit vector so each component is between -1 and 1, but colors need components of 0 and 1
        return 0.5*color(N.x() + 1, N.y() + 1, N.z() + 1); //spehre colored according to its normal vectors
    }

    //lerp between two values: blue to white
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a) * color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
}

int main() {
    
    //image
    auto aspect_ratio = 16.0/9.0;
    int image_width = 400;

    //Calculate the image height, and ensure that it's at least 1

    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    //viewport widths less than one are ok since they are real valued
    
    //camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto camera_center = point3(0,0,0);

    //calculate the vectors across the horizontal and down the veritcal viewport edges
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0,-viewport_height, 0);

    //calculate the horizontal and vertical delta vectors from pixel to pixel
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    //calculate the location of the upper left pixel
    auto viewport_upper_left = camera_center - vec3(0,0, focal_length) - viewport_u/2 - viewport_v /2;

    //a ray needs to pass through the center of the pixel not the top left, so we nudge inward by half a pixel in each direction
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v); 

    //render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    //pixels are written out in rows. 
    //outer loop 'j' is the row and we increment down till our we reach image_height
    //prints out the pixel from left to right
    for (int j = 0; j < image_height; j++) {
        //progress indicator
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r (camera_center, ray_direction);
            auto pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\r Done.          \n";
}