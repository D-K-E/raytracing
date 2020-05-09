// Author: Kaan Eraslan
// includes
#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <custom/vec3.hpp>

class RayCamera {
  // isin atan kamera/goz nesnesi
private:
  point3 origin;
  vec3 horizontal;
  vec3 vertical;
  point3 lower_left_corner;

public:
  RayCamera() {
    // constructor
    lower_left_corner = vec3(-2, -1, -1);
    origin = vec3(0, 0, 0);
    vertical = vec3(0, 2, 0);
    horizontal = vec3(4, 0, 0);
  }
  Ray get_ray(double u, double v) const {
    // get camera ray
    return Ray(origin, lower_left_corner + u * horizontal + v * vertical);
  }
};

#endif
