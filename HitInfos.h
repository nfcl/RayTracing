#ifndef _HITINFOS_H_
#define _HITINFOS_H_

#include "Vec3.h"
#include "Ray.h"
#include "Material.h"

class Material;

class HitInfos {
public:
    Point3 p;
    Vec3 normal;
    shared_ptr<Material> mat;
    double t;
    bool front_face;
    void set_face_normal(const Ray& r, const Vec3& outward_normal){
        front_face = Vec3::dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

#endif