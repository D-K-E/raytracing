// carpilabilir
#ifndef HITTABLE_HPP
#define HITTABLE_HPP
//
#include <custom/sabit.hpp> // vek3, ray
//

struct HitRecord {
  nokta3 point;
  vek3 normal;
  double dist;
  bool front_face;
  inline void set_face_normal(const Ray &r, const vek3 &out_normal) {
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
