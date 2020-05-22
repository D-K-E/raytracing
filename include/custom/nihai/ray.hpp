#ifndef RAY_HPP
#define RAY_HPP

#include <custom/nihai/vec3.hpp>

class Ray {
  /*N = C + mY
N: nokta
C: cikis yeri
m: mesafe/buyukluk
Y: yonu
   */
public:
  Ray() {}

  Ray(const point3 &orig, const vec3 &dir)
      : origin(orig), direction(dir), tm(0) {}
  Ray(const point3 &orig, const vec3 &dir, double t)
      : origin(orig), direction(dir), tm(t) {}

  point3 at(double dist) const { return direction * dist + origin; }

  point3 orig() const { return origin; }

  vec3 dir() const { return direction; }

  double time() const { return tm; }

public:
  vec3 direction;
  point3 origin;
  double tm;
};

#endif
