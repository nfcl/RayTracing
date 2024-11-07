#ifndef _AABB_H_
#define _AABB_H_

#include "Interval.h"
#include "Ray.h"

class AABB {
public:
	Interval x, y, z;
	AABB() {}
	AABB(
		const Interval& x,
		const Interval& y, 
		const Interval& z
	) : x(x), 
		y(y), 
		z(z) {}
	AABB(const Point3& a, const Point3& b) {
		x = (a[0] < b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
		y = (a[1] < b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
		z = (a[2] < b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
	}
	AABB(const AABB& box0, const AABB& box1) {
		x = Interval(box0.x, box1.x);
		y = Interval(box0.y, box1.y);
		z = Interval(box0.z, box1.z);
	}
	const Interval& AxisInterval(int n) const {
		return n == 0 ? x : (n == 1 ? y : z);
	}
	bool hit(const Ray& ray, Interval ray_t) const {
		const Point3& ray_origin = ray.origin();
		const Vec3& ray_direction = ray.direction();

		for (int axis = 0; axis < 3; ++axis) {
			const Interval& ax = AxisInterval(axis);
			const double adinv = 1.0 / ray_direction[axis];

			double t0 = (ax.min - ray_origin[axis]) * adinv;
			double t1 = (ax.max - ray_origin[axis]) * adinv;

			if (t0 < t1) {
				if (t0 > ray_t.min) ray_t.min = t0;
				if (t1 < ray_t.max) ray_t.max = t1;
			}
			else {
				if (t1 > ray_t.min) ray_t.min = t1;
				if (t0 < ray_t.max) ray_t.max = t0;
			}

			if (ray_t.max <= ray_t.min) {
				return false;
			}
		}

		return true;
	}
};

#endif // !_AABB_H_
