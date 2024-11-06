#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <iostream>

#include "Hittable.h"
#include "Color.h"
#include "Interval.h"
#include "Material.h"

class Camera {
public:

    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int samples_per_pixel = 100;
    int max_reflect_depth = 10;

    double vfov = 90;
    Point3 lookfrom = Point3(0, 0, 0);
    Point3 lookat = Point3(0, 0, -1);
    Vec3 vup = Vec3(0, 1, 0);

    void Render(const Hittable& world) {
        Initialize();
        
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                Color pixel_color(0, 0, 0);
                for(int sample = 0; sample < samples_per_pixel; ++sample) {
                    Ray ray = Get_ray(i, j);
                    pixel_color += Ray_Color(ray, max_reflect_depth, world);
                }
                write_color(std::cout, pixel_color * pixel_samples_scale);
            }
        }

        std::clog << "\rDone.                 \n";
    }
private:

    int image_height;
    double pixel_samples_scale;
    Point3 center;
    Point3 pixel100_loc;
    Vec3 pixel_delta_u;
    Vec3 pixel_delta_v;
    Vec3 u, v, w;

    void Initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;
        
        double focal_length = (lookfrom - lookat).length();
        double theta = degrees_to_radians(vfov);
        double h = std::tan(theta / 2);
        double viewport_height = 2 * h * focal_length;
        double viewport_width = viewport_height * (double(image_width)/image_height);

        w = (lookfrom - lookat).normalized();
        u = Vec3::cross(vup, w).normalized();
        v = Vec3::cross(w, u);

        Vec3 viewport_u = viewport_width * u;
        Vec3 viewport_v = viewport_height * -v;
    
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;
    
        Vec3 viewport_upper_left = center - (focal_length * w) - viewport_u / 2 - viewport_v / 2;
        pixel100_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    }
    Ray Get_ray(int i, int j) const {
        
        Vec3 offset = Sample_Square();
        Vec3 pixel_sample = pixel100_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);
        
        Point3 ray_origin = center;
        Vec3 ray_direction = pixel_sample - ray_origin;

        return Ray(ray_origin, ray_direction);
    }
    Vec3 Sample_Square() const {
        return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }
    Color Ray_Color(const Ray& ray, int depth, const Hittable& world) {
        if(depth <= 0) {
            return Color(0, 0, 0);
        }
        HitInfos infos;
        if(world.hit(ray, Interval(0.001, infinity), infos)) {
            Ray scattered;
            Color attenuation;
            if(infos.mat->scatter(ray, infos, attenuation, scattered)) {
                return attenuation * Ray_Color(scattered, depth - 1, world);
            }
            return Color(0, 0, 0);
        }
        Vec3 unit_direction = ray.direction().normalized();
        double a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }
};

#endif