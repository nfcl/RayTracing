#ifndef _METALMATERIAL_H_
#define _METALMATERIAL_H_

#include "Material.h"

class MetalMaterial : public Material {
public:
    MetalMaterial(const Color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}
    bool scatter(const Ray& r_in, const HitInfos& infos, Color& attenuation, Ray& scattered) const override {
        Vec3 reflected = reflect(r_in.direction(), infos.normal);
        reflected = reflected.normalized() + (fuzz * Vec3::random_unit());
        scattered = Ray(infos.p, reflected);
        attenuation = albedo;
        return (Vec3::dot(scattered.direction(), infos.normal) > 0);
    }
private:
    Color albedo;
    double fuzz;
};

#endif