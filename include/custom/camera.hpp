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
  RayCamera(vec3 pos, vec3 target, vec3 up, double vfov, double aspect_ratio) {
    origin = pos;
    double theta = degree_to_radian(vfov);
    double half_height = tan(theta / 2);
    double half_width = aspect_ratio * half_height;

    // w, v, u eksenleri
    vec3 w = to_unit(pos - target);
    vec3 u = to_unit(cross(up, w));
    vec3 v = cross(w, u);

    lower_left_corner = origin - half_width * u - half_height * v - w;
    horizontal = 2 * half_width * u;
    vertical = 2 * half_height * v;
  }
  Ray get_ray(double u, double v) const {
    // get camera ray
    return Ray(origin,
               lower_left_corner + u * horizontal + v * vertical - origin);
  }
};

class RayCameraLens : public RayCamera {
public:
  point3 origin;
  vec3 horizontal;
  vec3 vertical;
  point3 lower_left_corner;
  vec3 u, v, w;
  double lens_radius;

public:
  RayCameraLens() {}
  RayCameraLens(vec3 pos, vec3 target, vec3 up, double vfov,
                double aspect_ratio, double aperture, double focus_dist) {
    // camera lens constructor
    origin = pos;
    lens_radius = aperture / 2;
    double theta = degree_to_radian(vfov);
    double half_height = tan(theta / 2);
    double half_width = aspect_ratio * half_height;

    // w, v, u eksenleri
    w = to_unit(pos - target);
    u = to_unit(cross(up, w));
    v = cross(w, u);
    lower_left_corner = origin - half_width * focus_dist * u -
                        half_height * focus_dist * v - w * focus_dist;
    horizontal = 2 * half_width * focus_dist * u;
    vertical = 2 * half_height * focus_dist * v;
  }
  Ray get_ray(double t, double s) const {
    // get camera ray
    vec3 rd = lens_radius * random_in_unit_disk();
    vec3 offset = u * rd.x + v * rd.y;
    return Ray(origin + offset, lower_left_corner + t * horizontal +
                                    s * vertical - origin - offset);
  }
};

#endif
