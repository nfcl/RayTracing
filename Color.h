#ifndef _COLOR_H_
#define _COLOR_H_

#include "Interval.h"
#include "Vec3.h"

using Color = Vec3;

inline double linear_to_gamma(double linear_component) {
    if(linear_component > 0) {
        return std::sqrt(linear_component);
    }
    return 0;
}

void write_color(std::ostream& out, const Color& pixel_color) {
    auto r = linear_to_gamma(pixel_color.x());
    auto g = linear_to_gamma(pixel_color.y());
    auto b = linear_to_gamma(pixel_color.z());

    // Translate the [0,1] component values to the byte range [0,255].
    static const Interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif