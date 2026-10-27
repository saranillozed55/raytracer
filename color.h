#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"

using color = vec3;


inline double linear_to_gamma(double linear_component) {
    if(linear_component > 0) {
        return std::sqrt(linear_component);
    }
    return 0;
}


void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    //9.5, Transform from linear space to gamma space because its better for image storage and display
    //Standard image formats use nonlinear encoding like sRGB, so pixels display as intended on screen  
    r = linear_to_gamma(r);
    b = linear_to_gamma(b);
    g = linear_to_gamma(g);

    //translate the [0,1] component values o the byte range [0,255]
    static const interval intensity(0.000,0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    //write out the pixel color components
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif