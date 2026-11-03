#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include <thread>
#include <chrono> 
#include <vector>
#include <atomic>

class camera {
    public: 
        //The owning code will modify tghe camera's public variables through simple assignment
        //public camera parameters are here

        double aspect_ratio      = 1.0;  // Ratio of image width over height/default to 1.0
        int    image_width       = 100;  // Rendered image width in pixel count/default to 100
        int    samples_per_pixel = 10;   // Count of random samples for each pixel/default to 10
        int    max_depth         = 10;

        double vfov     = 90;              // vertical view angle (fov)
        point3 lookfrom = point3(0,0,0);   //Point camera is looking from
        point3 lookat   = point3(0,0,-1);  //point camera is looking at
        vec3   vup      = vec3(0,1,0);

        double defocus_angle = 0;   // variation angle of rays through each pixel - Depth Of Field (Smaller = Tighter)
        double focus_dist    = 10;  // distance from camera lookfrom point to plane of perfect focus

        void render(const hittable& world) {
        initialize();

        std::atomic<int> next_row = 0;
        std::vector<std::thread> threads;
                    
        auto startTime = std::chrono::steady_clock::now();
        std::clog << "Threads used: " << num_threads_supported << "\n";
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";        

        std::vector<color> framebuffer(image_width * image_height);
        
        auto lambda_render =[&]() {

                while(true) {
                        auto claimed = next_row.fetch_add(1);

                        if(claimed >= image_height) {
                                break;
                        }

                        for(int i = 0; i < image_width; i++) {

                                color pixel_color(0,0,0);

                                for(int sample = 0; sample < samples_per_pixel; sample++) {
                                        ray r = get_ray(i, claimed);
                                        pixel_color += ray_color(r, max_depth, world);
                                }

                                framebuffer[claimed * image_width + i] = pixel_color * pixel_samples_scale;                             
                        }
                }
        };

        for(int i = 0; i < num_threads_supported; i++) {
                threads.push_back(std::thread(lambda_render));
        }

        for(int i = 0; i < num_threads_supported;i ++) {
                threads[i].join();
        }

        for(std::size_t i = 0; i < framebuffer.size(); i++) {
                write_color(std::cout, framebuffer[i]);
        }

        auto endTime = std::chrono::steady_clock::now();
        std::clog << "Frame Buffer Size: " << framebuffer.size() << "\n";
        std::clog << "\rDone in: " << time_dt(startTime, endTime) << " " << "\n";
        
        }

    private: 
        //private camera variables here

        double pixel_samples_scale;  //color scale factor for a sum of pixel samples
        int    image_height;         // Rendered image height
        point3 center;               // Camera center
        point3 pixel00_loc;          // Location of pixel 0, 0
        vec3   pixel_delta_u;        // Offset to pixel to the right
        vec3   pixel_delta_v;        // Offset to pixel below
        vec3   u, v, w;              //camera frame basis vectors
        vec3   defocus_disk_u;       // defocus disk horizontal radius
        vec3   defocus_disk_v;       //defocus disk vertical radius
        const unsigned int num_threads_supported = std::max(1u, std::thread::hardware_concurrency()); // hardware_concurrency could return 0
        
        void initialize() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;


            center = lookfrom;

                        // Determine viewport dimensions and camera with adjustable fov 
                        // auto focal_length = (lookfrom - lookat).length(); // the distance between the camera center and the image plane
            auto theta           = degrees_to_radians(vfov);
            auto h               = std::tan(theta/2);
            auto viewport_height = 2.0 * h * focus_dist;
            auto viewport_width  = viewport_height * (double(image_width)/image_height);

                        //calculate the u,v,w unit basis vectors for the camera coordinate frame
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w,u);

                                                     // Calculate the vectors across the horizontal and down the vertical viewport edges
            auto viewport_u = viewport_width * u;    // vector across view port horizontal edge
            auto viewport_v = viewport_height * -v;  //vector down viewport vertical edge

                        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

                        // Calculate the location of the upper left pixel.
                        // w is negative, therefore if we subtract then we move forward by the focal_length and then we are at the center of the viewport

            auto viewport_upper_left = center - (focus_dist* w) - viewport_u/2 - viewport_v/2;
                 pixel00_loc         = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

                        //calculate the camera defocus disk basis vectors
            auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle/2));
                 defocus_disk_u = u * defocus_radius;
                 defocus_disk_v = v * defocus_radius;
        }

        color ray_color(const ray& r, int depth, const hittable& world) const {

                hit_record rec;

                if(depth <=0) {
                        return color(0,0,0);
                }

                //9.3, 0.001 to prevent floating point errors
                if(world.hit(r, interval(0.001, infinity), rec)) {

                //each pixel has its own attenuation and scattered
                ray scattered;
                color attenuation;

                if(rec.mat->scatter(r, rec, attenuation, scattered)) {
                    return attenuation * ray_color(scattered, depth -1, world);
                }
                
                return color(0,0,0);

                //hits sphere since we made the sphere hittable, in main.cpp hittable_list world

                //Lambertian spheres 9.4 - More rays scatter toward the normal at the intersection of the sphere with the ray
                // vec3 direction = rec.normal + random_unit_vector();
                
                // <val> * <stuff> : where val is the percent of reflectance
                // return 0.5 * ray_color(ray(rec.p, direction), depth-1, world); 
        }
                //lerp between two values: blue to white THE SKY
                vec3 unit_direction = unit_vector(r.direction());
                auto a              = 0.5*(unit_direction.y() + 1.0);
                return (1.0-a) * color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
        }

        ray get_ray(int i, int j) const {
                //construct a camera originating from the defocus disk and directed at randomly sampled point around the pixel location i,j
            
                auto offset       = sample_square();
                auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

                auto ray_origin    = (defocus_angle <= 0) ? center : defocus_disk_sample();
                auto ray_direction = pixel_sample - ray_origin;

                return ray(ray_origin, ray_direction);
        }

        vec3 sample_square() const  {
                        //returns the vecotr to a random point in the [-.5, -.5] - [+.5 + .5] unit square.
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        point3 defocus_disk_sample() const {
                        //returns a random point in the camera defocus disk
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }
        inline std::chrono::seconds time_dt(const auto& startTime, const auto& endTime) { 
                return std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
        }
};



#endif