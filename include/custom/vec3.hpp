#ifndef VEC3_HPP
#define VEC3_HPP

#include <custom/functions.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <ostream>

using vec3 = glm::dvec3;
using point3 = glm::dvec3;
using color = glm::dvec3;

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
  // yazim
  return out << v.x << ' ' << v.y << ' ' << v.z;
}
inline vec3 operator*(const vec3 &v, double d) {
  return vec3(v.x * d, v.y * d, v.z * d);
}
inline vec3 operator*(double d, const vec3 &v) {
  return vec3(v.x * d, v.y * d, v.z * d);
}
inline double dot(const vec3 &u, const vec3 &v) {
  // iccarpim
  return u.x * v.x + u.y * v.y + u.z * v.z;
}
inline vec3 cross(const vec3 &u, const vec3 &v) {
  // capraz carpim
  return vec3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z,
              u.x * v.y - u.y * v.x);
}
inline vec3 to_unit(vec3 v) { return glm::normalize(v); }
inline static vec3 random_vec() {
  // rastgele vektor uretiyor
  return vec3(random_double(), random_double(), random_double());
}
inline static vec3 random_vec(double min, double max) {
  // rastgele vektor uretiyor
  return vec3(random_double(min, max), random_double(min, max),
              random_double(min, max));
}
inline static vec3 random_in_unit_sphere() {
  // rastgele kurede bir yon veriyor
  while (true) {
    //
    vec3 v = random_vec(-1, 1);
    if (dot(v, v) >= 1) {
      continue;
    }
    return v;
  }
}
inline static vec3 random_unit_vector() {
  // unit vector
  double a = random_double(0, 2 * PI);
  double z = random_double(-1, 1);
  double r = sqrt(1 - z * z);
  return vec3(r * cos(a), r * sin(a), z);
}
inline static vec3 random_in_hemisphere(const vec3 &normal) {
  // normal ekseninde dagilan yon
  vec3 unit_sphere_dir = random_in_unit_sphere();
  if (dot(unit_sphere_dir, normal) > 0.0) {
    return unit_sphere_dir;
  } else {
    return -1 * unit_sphere_dir;
  }
}

#endif
