#ifndef _INTERVAL_H_
#define _INTERVAL_H_

#include "main.h"

class Interval {
public:
    Interval() : min(+infinity), max(-infinity) {}
    Interval(double min, double max) : min(min), max(max) {}
    Interval(const Interval& a, const Interval& b) {
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }
    double size() const {
        return max - min;
    }
    bool contains(double x) const {
        return min <= x && x <= max;
    }
    bool surrounds(double x) const {
        return min < x && x < max;
    }
    double clamp(double x) const {
        if(x < min) {
            return min;
        }
        else if (x > max){
            return max;
        }
        return x;
    }
    Interval expand(double delta) const {
        double padding = delta / 2;
        return Interval(min - padding, max + padding);
    }
    static const Interval empty, universe;
    double min;
    double max;
};

const Interval Interval::empty      = Interval(+infinity, -infinity);
const Interval Interval::universe   = Interval(-infinity, +infinity);

#endif