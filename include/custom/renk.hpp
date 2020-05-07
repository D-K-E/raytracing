#ifndef RENK_HPP
#define RENK_HPP

#include <custom/vec3.hpp>
#include <iostream>

int renk_cast(double renk_degeri) {
  return static_cast<int>(255.999 * renk_degeri);
}

void renk_yaz(std::ostream &out, renk pixel_rengi) {
  // renk degerlerini akisa yazar
  out << renk_cast(pixel_rengi.x) << ' ' << renk_cast(pixel_rengi.y) << ' '
      << renk_cast(pixel_rengi.z) << std::endl;
}

#endif
