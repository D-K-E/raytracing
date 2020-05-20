#ifndef COLOR_HPP
#define COLOR_HPP

#include <custom/sonraki/commons.hpp>
#include <iostream>

int cast_color(double d) {
  // renkleri double'dan int tipine donustur
  return static_cast<int>(255.999 * d);
}
int cast_color(double d, bool isClamped) {
  // renkleri double'dan int tipine donustur
  return static_cast<int>(256 * clamp(d, 0.0, 0.999));
}

void write_color(std::ostream &out, color pcolor) {
  out << cast_color(pcolor.x) << ' ' << cast_color(pcolor.y) << ' '
      << cast_color(pcolor.z) << std::endl;
}
void write_color(std::ostream &out, color pcolor, int samples_per_pixel) {
  double r, g, b;
  r = pcolor.x;
  g = pcolor.y;
  b = pcolor.z;
  // replace nans with zero since no nans are equal to each other
  r = (r != r) ? 0.0 : r;
  g = (g != g) ? 0.0 : g;
  b = (b != b) ? 0.0 : b;
  // scale sample
  r = sqrt(r * 1.0 / samples_per_pixel);
  g = sqrt(g * 1.0 / samples_per_pixel);
  b = sqrt(b * 1.0 / samples_per_pixel);
  out << cast_color(r, true) << ' ' << cast_color(g, true) << ' '
      << cast_color(b, true) << std::endl;
}

#endif
