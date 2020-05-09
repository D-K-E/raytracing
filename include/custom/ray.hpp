#ifndef RAY_HPP
#define RAY_HPP

#include <custom/vec3.hpp>

class Ray {
  /*N = C + mY
N: nokta
C: cikis yeri
m: mesafe/buyukluk
Y: yonu
   */
public:
  Ray() {}
  Ray(const point3 orig, const vec3 dir) {
    // 1. Constructor
    direction = dir;
    origin = orig;
  }
  point3 at(double dist) const { return direction * dist + origin; }

public:
  vec3 direction;
  point3 origin;
};

#endif
