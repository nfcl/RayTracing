#ifndef _METALMATERIAL_H_
#define _METALMATERIAL_H_

#include "Material.h"

class MetalMaterial : public Material {
public:
    MetalMaterial(const Color& albedo) : albedo(albedo) {}
    bool scatter(const Ray& r_in, const HitInfos& infos, Color& attenuation, Ray& scattered) const override {
        Vec3 reflected = reflect(r_in.direction(), infos.normal);
        scattered = Ray(infos.p, reflected);
        attenuation = albedo;
        return true;
    }
private:
    Color albedo;
};

#endif