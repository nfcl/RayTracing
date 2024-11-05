#ifndef _HITTABLE_H_
#define _HITTABLE_H_

#include "HitInfos.h"

class Hittable {
public:
    virtual ~Hittable() = default;
    virtual bool hit(const Ray& ray, Interval ray_t, HitInfos& infos) const = 0;
};

#endif