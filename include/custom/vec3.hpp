#ifndef VEC3_HPP
#define VEC3_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using nokta3 = glm::vec3;
using renk = glm::vec3;
using vek3 = glm::vec3;

void vektor_yaz(std::ostream &out, glm::vec3 v) {
  // renk degerlerini akisa yazar
  out << v.x << ' ' << v.y << ' ' << v.z;
}
#endif
