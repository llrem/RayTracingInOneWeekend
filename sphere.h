#ifndef _SPHERE_H
#define _SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
    sphere(point3 _center1, double _radius, shared_ptr<material> _material)
    : center1(_center1), radius(_radius), mat(_material), is_moving(false) {
        auto rvec = vec3(radius, radius, radius);
        bbox = aabb(center1 - rvec, center1 + rvec);
    }

    sphere(point3 _center1, point3 _center2, double _radius, shared_ptr<material> _material)
    : center1(_center1), radius(_radius), mat(_material), is_moving(true) {
        auto rvec = vec3(radius, radius, radius);
        aabb box1(_center1 - rvec, _center1 + rvec);
        aabb box2(_center2 - rvec, _center2 + rvec);
        bbox = aabb(box1, box2);

        center_vec = _center2 - _center1;
    }

    aabb bounding_box() const override {
        return bbox;
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        point3 center = is_moving ? sphere_center(r.time()) : center1;
        vec3 oc = r.origin() - center;
        auto a = dot(r.direction(), r.direction());
        auto b = 2.0 * dot(oc, r.direction());
        auto c = dot(oc, oc) - radius*radius;
        auto discriminant = b*b - 4*a*c;

        if(discriminant < 0) return false;

        auto t = (-b - sqrt(discriminant)) / (2.0 * a);
        if(!ray_t.surrounds(t)){
            t = (-b + sqrt(discriminant)) / (2.0 * a);
            if(!ray_t.surrounds(t)){
                return false;
            }
        }
        rec.t = t;
        rec.p = r.at(t);
        // 这里必须写成(r.at(t) - center) / radius; 不能为normalize(r.at(t) - center)
        // radius的值可能为负
        vec3 outward_normal = (r.at(t) - center) / radius;
        rec.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, rec.u, rec.v);
        rec.mat = mat;

        return true;
    }

private:
    point3 center1;
    double radius;
    shared_ptr<material> mat;
    bool is_moving;
    vec3 center_vec;
    aabb bbox;

    point3 sphere_center(double time) const {
        return center1 + time*center_vec;
    }

    static void get_sphere_uv(const point3& p, double& u, double& v) {
        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2*pi);
        v = theta / pi;
    }
};

#endif