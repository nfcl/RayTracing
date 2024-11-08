#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <iostream>
#include <thread>

#include "Hittable.h"
#include "Color.h"
#include "Interval.h"
#include "Material.h"
#include "ThreadPool.h"

using std::thread;
using std::atomic;

class Camera {
public:

    double aspect_ratio     = 16.0 / 9.0;
    int image_width         = 400;
    int samples_per_pixel   = 100;
    int max_depth   = 10;

    double vfov             = 90;
    Point3 lookfrom         = Point3(0, 0, 0);
    Point3 lookat           = Point3(0, 0, -1);
    Vec3 vup                = Vec3(0, 1, 0);

    double defocus_angle    = 0;
    double focus_dist       = 10;

    atomic<int> resultAdd   = 0;
    const Hittable* world;
    std::vector<std::vector<Color>> result;

    void Render(const Hittable& world) {
        Initialize();
        
        this->world = &world;

        ThreadPool pool(10);

        pool.init();

        resultAdd = image_height;

        result = std::vector<std::vector<Color>>(image_height);

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        std::clog << "\rScanlines remaining: " << resultAdd << ' ' << std::flush;

        for (int j = 0; j < image_height; j++) {
            auto future = pool.submit(std::bind(&Camera::_ThreadRender, this, std::placeholders::_1), j);
        }

        while (resultAdd != 0);

        for (int j = 0; j < image_height; ++j) {
            for (int i = 0; i < image_width; ++i) {
                write_color(std::cout, result[j][i]);
            }
        }

        std::clog << "\rDone.                 \n";

        pool.shutdown();
    }
    void _ThreadRender(int rowId) {
        result[rowId] = std::vector<Color>(image_width);
        for (int i = 0; i < image_width; i++) {
            Color pixel_color(0, 0, 0);
            for (int sample = 0; sample < samples_per_pixel; ++sample) {
                Ray ray = Get_ray(i, rowId);
                pixel_color += Ray_Color(ray, max_depth, *world);
            }
            result[rowId][i] = pixel_color * pixel_samples_scale;
        }
        resultAdd -= 1;
        std::clog << "\rScanlines remaining: " << resultAdd << ' ' << std::flush;
    }
private:

    int image_height;
    double pixel_samples_scale;
    Point3 center;
    Point3 pixel100_loc;
    Vec3 pixel_delta_u;
    Vec3 pixel_delta_v;
    Vec3 u, v, w;
    Vec3 defocus_disk_u;
    Vec3 defocus_disk_v;

    void Initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;
        
        double theta = degrees_to_radians(vfov);
        double h = std::tan(theta / 2);
        double viewport_height = 2 * h * focus_dist;
        double viewport_width = viewport_height * (double(image_width)/image_height);

        w = (lookfrom - lookat).normalized();
        u = Vec3::cross(vup, w).normalized();
        v = Vec3::cross(w, u);

        Vec3 viewport_u = viewport_width * u;
        Vec3 viewport_v = viewport_height * -v;
    
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;
    
        Vec3 viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel100_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        double defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }
    Ray Get_ray(int i, int j) const {
        Vec3 offset = Sample_Square();
        Vec3 pixel_sample = pixel100_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);
        
        Point3 ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        Vec3 ray_direction = pixel_sample - ray_origin;
        double ray_time = random_double();

        return Ray(ray_origin, ray_direction, ray_time);
    }
    Vec3 Sample_Square() const {
        return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }
    Point3 defocus_disk_sample() const {
        Vec3 p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
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