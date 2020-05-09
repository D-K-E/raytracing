#ifndef COMMONS_HPP
#define COMMONS_HPP

#include <cmath>
#include <cstdlib>
#include <functional>
#include <limits>
#include <memory>
#include <random>

// kullanilanlar
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// sabitler
const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.14159265358979;
// 3.1415926535897932385; da kullanilabilir

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

//
#include <custom/ray.hpp>
#include <custom/vec3.hpp>

#endif
