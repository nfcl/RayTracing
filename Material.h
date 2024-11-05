#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Hittable.h"
#include "Color.h"

class HitInfos;

class Material {
public:
    virtual ~Material() = default;
    virtual bool scatter(const Ray& r_in, const HitInfos& infos, Color& attenuation, Ray& scattered) const {
        return false;
    }
};

#endif