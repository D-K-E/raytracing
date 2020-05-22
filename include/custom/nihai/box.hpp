#ifndef BOX_HPP
#define BOX_HPP
//
#include <custom/nihai/commons.hpp>
#include <custom/nihai/hittables.hpp>
//
#include <custom/nihai/aarect.hpp>
//

class Box : public Hittable {
public:
  Box() {}
  Box(const point3 &p0, const point3 &p1, shared_ptr<Material> ptr);

  virtual bool hit(const Ray &r, double t0, double t1, HitRecord &rec) const;

  virtual bool bounding_box(double t0, double t1, Aabb &output_box) const {
    output_box = Aabb(box_min, box_max);
    return true;
  }

public:
  point3 box_min;
  point3 box_max;
  HittableList sides;
};

Box::Box(const point3 &p0, const point3 &p1, shared_ptr<Material> ptr) {
  box_min = p0;
  box_max = p1;

  sides.add(make_shared<XYRect>(p0.x, p1.x, p0.y, p1.y, p1.y, ptr));
  sides.add(make_shared<XYRect>(p0.x, p1.x, p0.y, p1.y, p0.y, ptr));

  sides.add(make_shared<XZRect>(p0.x, p1.x, p0.y, p1.y, p1.y, ptr));
  sides.add(make_shared<XZRect>(p0.x, p1.x, p0.y, p1.y, p0.y, ptr));

  sides.add(make_shared<YZRect>(p0.y, p1.y, p0.y, p1.y, p1.x, ptr));
  sides.add(make_shared<YZRect>(p0.y, p1.y, p0.y, p1.y, p0.x, ptr));
}

bool Box::hit(const Ray &r, double t0, double t1, HitRecord &rec) const {
  return sides.hit(r, t0, t1, rec);
}

#endif
