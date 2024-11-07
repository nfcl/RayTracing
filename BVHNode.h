#ifndef _BVHNODE_H_
#define _BVHNODE_H_

#include "AABB.h"
#include "Hittable.h"
#include "HittableList.h"

#include <algorithm>
#include <functional>

class BVHNode : public Hittable {
public:
	BVHNode(HittableList list) : BVHNode(list.objects, 0, list.objects.size()) {}
	BVHNode(std::vector<shared_ptr<Hittable>>& objects, size_t start, size_t end) {
		int axis = random_int(0, 2);

		auto comparator = std::bind(box_compare, std::placeholders::_1, std::placeholders::_2, axis);

		size_t object_span = end - start;

		if (object_span == 1) {
			left = right = objects[start];
		}
		else if (object_span == 2) {
			left = objects[start];
			right = objects[start + 1];
		}
		else {
			std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

			size_t mid = start + object_span / 2;
			left = make_shared<BVHNode>(objects, start, mid);
			right = make_shared<BVHNode>(objects, mid, end);
		}

		bbox = AABB(left->BoundingBox(), right->BoundingBox());
	}
	bool hit(const Ray& ray, Interval ray_t, HitInfos& infos) const override {
		if (!bbox.hit(ray, ray_t)) {
			return false;
		}

		bool hit_left = left->hit(ray, ray_t, infos);
		bool hit_right = right->hit(ray, Interval(ray_t.min, hit_left ? infos.t : ray_t.max), infos);

		return hit_left || hit_right;
	}
	AABB BoundingBox() const override {
		return bbox;
	}
private:
	shared_ptr<Hittable> left;
	shared_ptr<Hittable> right;
	AABB bbox;

	static bool box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis_index) {
		Interval a_axis_interval = a->BoundingBox().AxisInterval(axis_index);
		Interval b_axis_interval = b->BoundingBox().AxisInterval(axis_index);
		return a_axis_interval.min < b_axis_interval.min;
	}
};

#endif