#ifndef COMMONS_HPP
#define COMMONS_HPP

#include <cmath>
#include <cstdlib>
#include <functional>
#include <limits>
#include <memory>
#include <numeric>
#include <random>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double INF = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932;
/*

inline double degree_to_radian(double degrees) { return degrees * PI / 180.0; }

inline double clamp(double x, double min, double max) {
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

inline double random_double() { return rand() / (RAND_MAX + 1.0); }

inline double random_double(double min, double max) {
  // Returns a random real in [min,max).
  return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
  // Returns a random integer in [min,max].
  return static_cast<int>(random_double(min, max + 1));
}
*/

inline double degree_to_radian(double degree) {
  //
  return degree * PI / 180;
}

inline double random_double(double min, double max) {
  // random double number in range [min, max]
  static std::uniform_real_distribution<double> distr(min, max);
  static thread_local std::mt19937 gen;
  static std::function<double()> rand_gen = std::bind(distr, gen);
  return rand_gen();
}

inline double random_double() { return random_double(0, 1); }
inline int random_int() {
  // from so: https://stackoverflow.com/a/21238187
  return static_cast<int>(random_double());
}
inline int random_int(int min, int max) {
  return static_cast<int>(random_double(min, max));
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

#include <custom/nihai/vec3.hpp>
//
#include <custom/nihai/ray.hpp>

//
#endif
