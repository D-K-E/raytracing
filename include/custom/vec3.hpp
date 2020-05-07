#ifndef VEC3_HPP
#define VEC3_HPP

#include <glm/vec3.hpp>
#include <iostream>

using nokta3 = glm::dvec3;
using renk = glm::dvec3;
using vek3 = glm::dvec3;

void vektor_yaz(std::ostream &out, // readable
        glm::dvec3 v) {
  // renk degerlerini akisa yazar
  out << v.x << ' ' << v.y << ' ' << v.z;
}
#endif
