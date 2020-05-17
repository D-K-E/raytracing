//
#include <custom/nihai/commons.hpp>
//
#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdlib.h>
//

int main() {
  /*
    We get very quickly near π, and then more slowly zero in on it. This is an
    example of the Law of Diminishing Returns, where each sample helps less
    than the last. This is the worst part of MC. We can mitigate this
    diminishing return by stratifying the samples (often called jittering),
    where instead of taking random samples, we take a grid and take one sample
    within each: This changes the sample generation, but we need to know how
    many samples we are taking in advance because we need to know the grid.
  */
  int sqrtN = 1200; // number of points
  int inside_circle = 0;
  int inside_circle_stratified = 0;
  int radius_circle = 1;
  for (int i = 0; i < sqrtN; i++) {
    for (int j = 0; j < sqrtN; j++) {
      // generating random points regular way
      double x =
          random_double(-radius_circle, radius_circle); // -1 1 comes from
      double y = random_double(-radius_circle, radius_circle);
      if (x * x + y * y < 1) {
        inside_circle += 1;
      }
      x = 2 * ((i + random_double()) / sqrtN) - 1;
      y = 2 * ((j + random_double()) / sqrtN) - 1;
      if (x * x + y * y < 1) {
        inside_circle_stratified += 1;
      }
    }
  }
  double N = static_cast<double>(sqrtN) * sqrtN;
  std::cout << std::fixed << std::setprecision(12);
  std::cout << "Pi estimate regular: " << 4 * double(inside_circle) / N
            << std::endl;
  std::cout << "Pi estimate stratified: "
            << 4 * double(inside_circle_stratified) / N << std::endl;
}
