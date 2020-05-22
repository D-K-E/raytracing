#ifndef HITTABLES_HPP
#define HITTABLES_HPP

//
#include <custom/nihai/commons.hpp>
//
#include <custom/nihai/hittable.hpp>
//
#include <memory>
#include <vector>

class HittableList : public Hittable {
public:
  HittableList() {}
  HittableList(shared_ptr<Hittable> object) { add(object); }

  void clear() { objects.clear(); }
  void add(shared_ptr<Hittable> object) { objects.push_back(object); }

  virtual bool hit(const Ray &r, double tmin, double tmax,
                   HitRecord &rec) const;
  virtual bool bounding_box(double t0, double t1, Aabb &output_box) const;
  virtual double pdf_value(const vec3 &o, const vec3 &v) const;
  virtual vec3 random(const vec3 &o) const;

public:
  std::vector<shared_ptr<Hittable>> objects;
};

bool HittableList::hit(const Ray &r, double t_min, double t_max,
                       HitRecord &rec) const {
  HitRecord temp_rec;
  auto hit_anything = false;
  auto closest_so_far = t_max;

  for (const auto &object : objects) {
    if (object->hit(r, t_min, closest_so_far, temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.dist;
      rec = temp_rec;
    }
  }

  return hit_anything;
}

bool HittableList::bounding_box(double t0, double t1, Aabb &output_box) const {
  if (objects.empty())
    return false;

  Aabb temp_box;
  bool first_true = objects[0]->bounding_box(t0, t1, temp_box);

  if (!first_true)
    return false;

  output_box = temp_box;

  for (const auto &object : objects) {
    if (!object->bounding_box(t0, t1, temp_box))
      return false;
    output_box = surrounding_box(output_box, temp_box);
  }

  return true;
}

double HittableList::pdf_value(const point3 &o, const vec3 &v) const {
  auto weight = 1.0 / objects.size();
  auto sum = 0.0;

  for (const auto &object : objects)
    sum += weight * object->pdf_value(o, v);

  return sum;
}

vec3 HittableList::random(const vec3 &o) const {
  auto int_size = static_cast<int>(objects.size());
  return objects[random_int(0, int_size - 1)]->random(o);
}

#endif
