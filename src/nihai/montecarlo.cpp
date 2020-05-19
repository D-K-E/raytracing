//
#include <custom/nihai/commons.hpp>
//
#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdlib.h>
//
/*
   Monte carlo method for computing the integral of a function f over interval
   [a,b] is: Let 	x_1 , . . . , x_n be n randomly chosen points in the
   interval [a, b]. Then Y = (b − a)(1/n) \sum_{i=1}^n f(x_i) is a random +
   value
   whose average (over many different choices of x_1 , . . . , x_n) is the
   integral [a,b] f.
   We can compute the estimate for f in two different ways.

   - We can sample uniformly the interval. In this case the variance between
   our estimate and the function would decrease as the number of samples
   increase.

   - We can sample non uniformly the interval in a way that where the function
   evaluates to a larger value. It is like choosing points around the peaks of
   the function

   The second sampling method is called importance sampling.
   Let's see an example:

   suppose we are trying to integrate cos^2(t) where t is the colatitude, that
   is the angle between the radius and the z axis. We have a form:

   \int cos^2(t) dt

   Importance sampling indicates that we can sample this function: cos^2(t)
   using another distribution defined on another direction. Thus we can have
   something like
   \int_{S^2} (cos^2(t) / p(k)) . p(k) dt

   S^2 being the set of directions in unit sphere.

   The problem is how do we define k and what is p(). The k can be defined
   using longitude and colatitude (phi, theta) on unit sphere, since all
   directions are defined that way on unit sphere.
   Now what is the density of directions on unit sphere ? It is the inverse of
   the area of the sphere. Area of sphere is 4\pi r^2, its inverse is
   1/4*pi*r^2. For unit sphere the radius is 1. Then the density distribution
   is 1/4*pi

    */

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
