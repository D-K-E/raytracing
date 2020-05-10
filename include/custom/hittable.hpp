// carpilabilir
#ifndef HITTABLE_HPP
#define HITTABLE_HPP
//
#include <custom/commons.hpp> // vek3, ray
//
class Material;

struct HitRecord {
  point3 point;
  vec3 normal;
  shared_ptr<Material> mat_ptr;
  double dist;
  bool front_face;
  inline void set_face_normal(const Ray &r, const vec3 &out_normal) {
    // gelen dikme on yuze mi arka yuze mi
    front_face = dot(r.direction, out_normal) < 0;
    normal = front_face ? out_normal : -1 * out_normal;
  }
};

class Hittable {
public:
  virtual bool hit(const Ray &r, double dist_min, double dist_max,
                   HitRecord &rec) const = 0;
};

#endif
