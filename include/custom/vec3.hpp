#ifndef VEC3_HPP
#define VEC3_HPP

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

#endif
