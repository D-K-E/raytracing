#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <cmath>
#include <cstdlib>
#include <custom/constants.hpp>
#include <functional>
#include <numeric>
#include <random>

inline double degree_to_radian(double degree) {
  //
  return degree * PI / 180;
}
inline int random_int() {
  // from so: https://stackoverflow.com/a/21238187
  static thread_local std::mt19937 generator;
  std::uniform_int_distribution<int> distribution(0, 1);
  return distribution(generator);
}
inline int random_int(const int &min, const int &max) {
  // from so: https://stackoverflow.com/a/21238187
  static thread_local std::mt19937 generator;
  std::uniform_int_distribution<int> distribution(min, max);
  return distribution(generator);
}

//
//
inline double random_double(double min, double max) {
  // random double number in range [min, max]
  static std::uniform_real_distribution<double> distr(min, max);
  static thread_local std::mt19937 gen;
  static std::function<double()> rand_gen = std::bind(distr, gen);
  return rand_gen();
}

inline double random_double() { return random_double(0, 1); }
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
