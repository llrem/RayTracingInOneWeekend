#ifndef _RAY_H
#define _RAY_H

#include "vec3.h"

class ray {
public:
    ray() {}

    ray(const point3& origin, const vec3& direction, double time = 0.0)
    : ori(origin), dir(direction), tm(time) {}

    point3 origin() const {
        return ori; 
    }

    vec3 direction() const { 
        return dir; 
    }

    double time() const {
        return tm;
    }

    point3 at(double t) const {
        return ori + t * dir;
    }

private:
    point3 ori;
    vec3 dir;
    double tm;
};

#endif