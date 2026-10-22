#include <iostream>
#include "vec3.h"
#include "color.h"

int main() {
    
    //image
    int image_width = 256;
    int image_height = 256;

    //render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    //pixels are written out in rows. 
    //outer loop 'j' is the row and we increment down till our we reach image_height
    //prints out the pixel from left to right
    for (int j = 0; j < image_height; j++) {
        //progress indicator
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

        for (int i = 0; i < image_width; i++) {
            auto pixel_color = color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\r Done.          \n";
}