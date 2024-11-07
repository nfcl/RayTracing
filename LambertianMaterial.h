#ifndef _LAMBERTIANMATERIAL_H_
#define _LAMBERTIANMATERIAL_H_

#include "Material.h"

class LambertianMaterial : public Material {
public:
    LambertianMaterial(const Color& albedo) : albedo(albedo) {}
    bool scatter(const Ray& r_in, const HitInfos& infos, Color& attenuation, Ray& scattered) const override {
        Vec3 scatter_direction = infos.normal + Vec3::random_unit();

        if(scatter_direction.near_zero()) {
            scatter_direction = infos.normal;
        }
        
        scattered = Ray(infos.p, scatter_direction, r_in.time());
        attenuation = albedo;
        return true;
    }
private:
    Color albedo;
};

#endif