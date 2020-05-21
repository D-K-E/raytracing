//
#include <custom/nihai/commons.hpp>
//
#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdlib.h>
//

double sqr(double x) { return x * x; }

inline double xpdf(double x) { return 0.5 * x; }

int main() {
  /*
     integrating a function
  */
  int N = 10000; // number of samples
  double s = 0;  // total sum
  for (int i = 0; i < N; i++) {
    //
    double x = sqrt(random_double(0, 4)); // x is a value between 0 and 4
    s += sqr(x) / xpdf(x);                // sqr function
  }
  double average = s / N;
  std::cout << std::fixed << std::setprecision(12);
  std::cout << "I = " << average << std::endl;
}
