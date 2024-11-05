#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Hittable.h"

class Sphere : public Hittable {
public:
    Sphere(const Point3& center, double radius, shared_ptr<Material> mat) : center(center), radius(std::fmax(0, radius)), mat(mat) {}
    bool hit(const Ray& ray, Interval ray_t, HitInfos& infos) const override {
        Vec3 oc = center - ray.origin();
        double a = ray.direction().length_squared();
        double h = Vec3::dot(ray.direction(), oc);
        double c = oc.length_squared() - radius * radius;
        double discriminant = h * h - a * c;

        if(discriminant < 0){
            return false;
        }

        double sqrtd = std::sqrt(discriminant);

        double root = (h - sqrtd) / a;
        if(!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if(!ray_t.surrounds(root)){
                return false;
            }
        }

        infos.t = root;
        infos.p = ray.at(infos.t);
        Vec3 outward_normal = (infos.p - center) / radius;
        infos.set_face_normal(ray, outward_normal);
        infos.mat = mat;

        return true;
    }
private:
    Point3 center;
    double radius;
    shared_ptr<Material> mat;
};

#endif