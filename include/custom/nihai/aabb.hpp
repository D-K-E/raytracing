#ifndef AABB_HPP
#define AABB_HPP

#include <custom/nihai/commons.hpp>

class Aabb {
public:
  Aabb() {}
  Aabb(const point3 &a, const point3 &b) {
    _min = a;
    _max = b;
  }

  point3 min() const { return _min; }
  point3 max() const { return _max; }

  bool hit(const Ray &r, double tmin, double tmax) const {
    for (int a = 0; a < 3; a++) {
      auto t0 = fmin((_min[a] - r.orig()[a]) / r.dir()[a],
                     (_max[a] - r.orig()[a]) / r.dir()[a]);
      auto t1 = fmax((_min[a] - r.orig()[a]) / r.dir()[a],
                     (_max[a] - r.orig()[a]) / r.dir()[a]);
      tmin = fmax(t0, tmin);
      tmax = fmin(t1, tmax);
      if (tmax <= tmin)
        return false;
    }
    return true;
  }

  double area() const {
    auto a = _max.x - _min.x;
    auto b = _max.y - _min.y;
    auto c = _max.z - _min.z;
    return 2 * (a * b + b * c + c * a);
  }

  int longest_axis() const {
    auto a = _max.x - _min.x;
    auto b = _max.y - _min.y;
    auto c = _max.z - _min.z;
    if (a > b && a > c)
      return 0;
    else if (b > c)
      return 1;
    else
      return 2;
  }

public:
  point3 _min;
  point3 _max;
};

Aabb surrounding_box(Aabb box0, Aabb box1) {
  vec3 small(fmin(box0.min().x, box1.min().x), fmin(box0.min().y, box1.min().y),
             fmin(box0.min().z, box1.min().z));

  vec3 big(fmax(box0.max().x, box1.max().x), fmax(box0.max().y, box1.max().y),
           fmax(box0.max().z, box1.max().z));

  return Aabb(small, big);
}

#endif
