#ifndef BOX_HPP
#define BOX_HPP
//
#include <custom/sonraki/commons.hpp>
#include <custom/sonraki/hittables.hpp>
#include <custom/sonraki/material.hpp>
//
#include <custom/sonraki/aarect.hpp>
//

class Box : public Hittable {
public:
  point3 bmin;
  point3 bmax;
  HittableList sides;

public:
  Box(){};
  Box(const point3 &p0, const point3 &p1, shared_ptr<Material> ptr)
      : bmin(p0), bmax(p1) {
    //
    sides.add(make_shared<XYRect>(p0.x, p1.x, p0.y, p1.y, p1.z, ptr));
    sides.add(make_shared<XYRect>(p0.x, p1.x, p0.y, p1.y, p0.z, ptr));

    sides.add(make_shared<XZRect>(p0.x, p1.x, p0.z, p1.z, p1.y, ptr));
    sides.add(make_shared<XZRect>(p0.x, p1.x, p0.z, p1.z, p0.y, ptr));

    sides.add(make_shared<YZRect>(p0.y, p1.y, p0.z, p1.z, p1.x, ptr));
    sides.add(make_shared<YZRect>(p0.y, p1.y, p0.z, p1.z, p0.x, ptr));
  }
  bool hit(const Ray &r, double t0, double t1, HitRecord &rec) const {
    return sides.hit(r, t0, t1, rec);
  }
  virtual bool bounding_box(double t0, double t1, Aabb &output_box) const {
    output_box = Aabb(bmin, bmax);
    return true;
  }
  point3 min() const { return bmin; }
  point3 max() const { return bmax; }
};

#endif
