#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <cmath>
#include <cstdlib>
#include <custom/constants.hpp>
#include <functional>
#include <random>

inline double degree_to_radian(double degree) {
  //
  return degree * PI / 180;
}

//
inline double random_double() {
  // random double number
  static std::uniform_real_distribution<double> distr(0.0, 1.0);
  static std::mt19937 gen;
  static std::function<double()> rand_gen = std::bind(distr, gen);
  return rand_gen();
}
//
inline double random_double(double min, double max) {
  // random double number in range [min, max]
  return min + (max - min) * random_double();
}
//
inline double clamp(double x, double min, double max) {
  //
  bool xmin = x < min;
  bool xmax = x > max;
  if (xmin) {
    return min;
  }
  if (xmax) {
    return max;
  }
  return x;
}

#endif
