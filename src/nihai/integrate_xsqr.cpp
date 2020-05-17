//
#include <custom/nihai/commons.hpp>
//
#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdlib.h>
//

double sqr(double x) { return x * x; }

int main() {
  /*
     integrating a function
  */
  int N = 10000; // number of samples
  double s = 0;  // total sum
  for (int i = 0; i < N; i++) {
    //
    double x = random_double(0, 2); // x is a value between 0 and 2
    s += sqr(x);                    // sqr function
  }
  double average = s / N;
  std::cout << std::fixed << std::setprecision(12);
  std::cout << "I = " << 2 * average << std::endl;
}
/*
    In graphics, we often have functions we can evaluate but can’t write down
    explicitly, or functions we can only probabilistically evaluate. That is
    in fact what the ray tracing ray_color() function of the last two books is
    — we don’t know what color is seen in every direction, but we can
    statistically estimate it in any given dimension.

    One problem with the random program we wrote in the first two books is
    that small light sources create too much noise. This is because our
    uniform sampling doesn’t sample these light sources often enough. Light
    sources are only sampled if a ray scatters toward them, but this can be
    unlikely for a small light, or a light that is far away. We could lessen
    this problem if we sent more random samples toward this light, but this
    will cause the scene to be inaccurately bright. We can remove this
    inaccuracy by downweighting these samples to adjust for the over-sampling.
    How we do that adjustment? To do that, we will need the concept of a
    probability density function.

 */
