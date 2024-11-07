#ifndef _HITTABLELIST_H_
#define _HITTABLELIST_H_

#include "Hittable.h"
#include "AABB.h"

#include <vector>
#include <memory>

using std::make_shared;
using std::shared_ptr;

class HittableList : public Hittable {
public:
    
    std::vector<shared_ptr<Hittable>> objects;

    HittableList() {}
    HittableList(shared_ptr<Hittable> object) {
        Add(object);
    }
    void Clear() {
        objects.clear();
    }
    void Add(shared_ptr<Hittable> object) {
        objects.push_back(object);
        bbox = AABB(bbox, object->BoundingBox());
    }
    bool hit(const Ray& ray, Interval ray_t, HitInfos& infos) const override {
        HitInfos temp_infos;
        bool hit_anything = false;
        double closest_so_far = ray_t.max;
        for(const auto& object : objects){
            if(object->hit(ray, Interval(ray_t.min, closest_so_far), temp_infos)) {
                hit_anything = true;
                closest_so_far = temp_infos.t;
                infos = temp_infos;
            }
        }
        return hit_anything;
    }
    AABB BoundingBox() const override {
        return bbox;
    }
private:
    AABB bbox;
};

#endif