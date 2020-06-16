#ifndef HITTABLES_HPP
#define HITTABLES_HPP

//
#include <custom/sonraki/commons.hpp>
//
#include <custom/sonraki/hittable.hpp>
#include <memory>
#include <vector>
//

class HittableList : public Hittable {
public:
  std::vector<std::shared_ptr<Hittable>> objects;
  HittableList() {}
  HittableList(shared_ptr<Hittable> object) { add(object); }
  void add(shared_ptr<Hittable> object) { objects.push_back(object); }
  void clear() { objects.clear(); }
  virtual bool hit(const Ray &r, double dist_min, double dist_max,
                   HitRecord &record) const {
    // isin herhangi bir objeye vurdu mu vurmadi mi onu kontrol eden
    // fonksiyon
    HitRecord temp;
    bool hit_ = false;
    double current_closest = dist_max;
    for (const shared_ptr<Hittable> &object : objects) {
      // int i=0; i < objects.size(); i++
      if (object->hit(r, dist_min, current_closest, temp)) {
        hit_ = true;
        current_closest = temp.dist;
        record = temp;
      }
    }
    return hit_;
  }
  virtual bool bounding_box(double t1, double t2, Aabb &output_box) const {
    //
    if (objects.empty()) {
      return false;
    }
    bool fbox = true;
    Aabb temp_box;
    for (const shared_ptr<Hittable> object : objects) {
      if (object->bounding_box(t1, t2, temp_box) == false) {
        return false;
      }
      output_box = fbox ? temp_box : surrounding_box(output_box, temp_box);
      fbox = false;
    }
    return true;
  }
};

#endif
