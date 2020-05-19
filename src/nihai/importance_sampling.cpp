//
#include <custom/nihai/commons.hpp>
//
#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdlib.h>
//
/*
   Importance sampling is a general technique for estimating properties of a
   particular distribution, while only having samples generated from a
   different distribution than the distribution of interest

   A random variable is a function whose domain is the sample space and whose
   codomain is a measurable set. In many cases the codomain is R. An expected
   value of a random variable X, ie function X,is \sum_{s \in S} p(s)X(s)
   where S is the sample space. It reads as sum of the probability of sample s
   given and the value of random variable X evaluated for sample s.

   In importance sampling we assume the following scenario:
   I have a function f:  [a, b] → R is an integrable real-valued
   function. X is a random variable whose domain is the interval [a, b], with distribution
   g. f(X)/g(X) is a random variable whose expected value, or mean value (they
   imply the same thing), E[f(x)] = \int_a^b
   f(x). Meaning that the expected value of f(x) coincides with that of
   f(x)/g(x) when we plug f(x)/g(x) to the evaluation of the expected value of
   the random variable X. Now if we can find a constant ratio between them
   something like f(x)/g(x) = some constant C, our estimation would be
   unbiased and we would live happily here after.
   But for us to know find that constant we would need to know the f(x), that
   is the answer we are trying to solve. However, if we can choose a g(x) that
   gets larger when f(x) gets larger and gets lower when f(x) gets lower, we
   would decrease the variance of our estimations.

   How do we choose g ? You simply choose a probability density function (PDF)
   that is evaluated as non zero within the interval of f.
    */

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
