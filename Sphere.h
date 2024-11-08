#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Hittable.h"

class Sphere : public Hittable {
public:
    Sphere(
        const Point3& static_center,
        double radius,
        shared_ptr<Material> mat
    ) : center(static_center, Vec3(0, 0, 0)),
        radius(std::fmax(0, radius)),
        mat(mat)
    {
        Vec3 rvec = Vec3(radius, radius, radius);
        bbox = AABB(static_center - rvec, static_center + rvec);
    }
    Sphere(
        const Point3& center1,
        const Point3& center2,
        double radius,
        shared_ptr<Material> mat
    ) : center(center1, center2 - center1),
        radius(std::fmax(0, radius)),
        mat(mat) 
    {
        Vec3 rvec = Vec3(radius, radius, radius);
        AABB box1(center.at(0) - rvec, center.at(0) + rvec);
        AABB box2(center.at(1) - rvec, center.at(1) + rvec);
        bbox = AABB(box1, box2);
    }
    bool hit(const Ray& ray, Interval ray_t, HitInfos& infos) const override {
        Point3 current_center = center.at(ray.time());
        Vec3 oc = current_center - ray.origin();
        double a = ray.direction().length_squared();
        double h = Vec3::dot(ray.direction(), oc);
        double c = oc.length_squared() - radius * radius;
        double discriminant = h * h - a * c;

        if (discriminant < 0) {
            return false;
        }

        double sqrtd = std::sqrt(discriminant);

        double root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                return false;
            }
        }

        infos.t = root;
        infos.p = ray.at(infos.t);
        Vec3 outward_normal = (infos.p - current_center) / radius;
        infos.set_face_normal(ray, outward_normal);
        infos.mat = mat;

        return true;
    }
    AABB BoundingBox() const override {
        return bbox;
    };
private:
    Ray center;
    double radius;
    shared_ptr<Material> mat;
    AABB bbox;
};

#endif