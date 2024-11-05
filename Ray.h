#ifndef _RAY_H_
#define _RAY_H_

#include "vec3.h"

class Ray {
public:
    Ray() {}
    Ray(const Point3& origin, const Vec3& direction): _origin(origin), _direction(direction) {}
    const Point3& origin() const {
        return this->_origin;
    }
    const Point3& direction() const {
        return this->_direction;
    }
    Point3 at(double t) const {
        return this->_origin + t * this->_direction;
    }
private:
    Point3 _origin;
    Vec3 _direction;
};

#endif