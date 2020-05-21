//
#include <custom/nihai/commons.hpp>
//
#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdlib.h>
//

double cosqr(vec3 v) { return v.z * v.z; }

inline double xpdf(vec3 x) { return 1 / (4 * PI); }

int main() {
  /*
     integrating a function
  */
  int sampleNb = 1000;
  double s = 0.0;
  for (int i = 0; i < sampleNb; i++) {
    vec3 direction = random_unit_vector();
    double csqr = cosqr(direction);
    s += csqr / xpdf(direction); // notice the pdf takes a direction as arg.
  }
  std::cout << std::fixed << std::setprecision(12);
  double average = s / sampleNb;
  std::cout << "I = " << average << std::endl;
}
