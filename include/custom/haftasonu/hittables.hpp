#ifndef HITTABLES_HPP
#define HITTABLES_HPP

//
#include <custom/haftasonu/commons.hpp>
//
#include <custom/haftasonu/hittable.hpp>
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
};

#endif
