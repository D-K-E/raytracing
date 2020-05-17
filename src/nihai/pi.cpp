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
    Now, suppose you pick random points inside the square. The fraction of
    those random points that end up inside the circle should be proportional
    to the area of the circle. The exact fraction should in fact be the ratio
    of the circle area to the square area. Fraction:

    πr^2 / (2r)^2 = π / 4

    Since the r cancels out, we can pick whatever is computationally
    convenient. Let’s go with r=1, centered at the origin:
   */
  int N = 1000; // number of points
  int inside_circle = 0;
  int radius_circle = 1;
  for (int i = 0; i < N; i++) {
    // generating random points
    double x = random_double(-radius_circle, radius_circle); // -1 1 comes from
    double y = random_double(-radius_circle, radius_circle);
    if (x * x + y * y < 1) {
      inside_circle += 1;
    }
  }
  std::cout << std::fixed << std::setprecision(12);
  std::cout << "Pi estimate: " << 4 * double(inside_circle) / N << std::endl;
}
