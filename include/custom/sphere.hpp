// Kure objesi
#ifndef SPHERE_HPP
#define SPHERE_HPP

//
#include <custom/commons.hpp> // vek3, ray
//
#include <custom/hittable.hpp>
//

class Sphere : public Hittable {
public:
  Sphere() {}
  Sphere(point3 cent, double r) {
    center = cent;
    radius = r;
  }
  bool hit(const Ray &r, double dist_min, double dist_max,
           HitRecord &record) const {
    // kureye isin vurdu mu onu test eden fonksiyon
    vec3 origin_to_center = r.origin - center;
    double a = dot(r.direction, r.direction);
    double half_b = dot(origin_to_center, r.direction);
    double c = dot(origin_to_center, origin_to_center) - radius * radius;
    double isHit = half_b * half_b - a * c;
    double margin;
    if (isHit > 0) {
      double root = sqrt(isHit);
      margin = (-1 * half_b - root) / a;
      if (margin < dist_max && margin > dist_min) {
        record.dist = margin;
        record.point = r.at(record.dist);
        vec3 out_normal = (record.point - center) / radius;
        record.set_face_normal(r, out_normal);
        return true;
      }
      margin = (-1 * half_b + root) / a;
      if (margin < dist_max && margin > dist_min) {
        record.dist = margin;
        record.point = r.at(record.dist);
        vec3 out_normal = (record.point - center) / radius;
        record.set_face_normal(r, out_normal);
        return true;
      }
    }
    return false;
  }
  point3 center;
  double radius;
};

#endif
