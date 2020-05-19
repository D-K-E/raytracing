//
#include <custom/nihai/commons.hpp>
//
#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdlib.h>
//
/*
A random variable is a function whose domain is the sample space and whose
   codomain is a measurable set.  A probability distribution is a mathematical
   function that has a sample space as its input, and gives a probability as
   its output. In many cases the codomain is R.
   When the codomain is discrete the random variable is called a discrete
   random variable. Its distribution is a discrete probability distribution.
   If the codomain is uncountably infinite it is called continuous random
   variable, when it is absolutely continuous, its distribution is can be
   described by a probability density function which assigns probabilities to
   intervals, that is each individual sample has probability of 0, but
   intervals can be bigger than 0.

    https://en.wikipedia.org/wiki/Probability_density_function
    Probability density function is is a function whose value at any given sample (or
    point) in the sample space (the set of possible values taken by the random
    variable) can be interpreted as providing a relative likelihood that the
    value of the random variable would equal that sample.[2] In other words,
    while the absolute likelihood for a continuous random variable to take on
    any particular value is 0 (since there are an infinite set of possible
    values to begin with), the value of the PDF at two different samples can
    be used to infer, in any particular draw of the random variable, how much
    more likely it is that the random variable would equal one sample compared
    to the other sample.

    In a more precise sense, the PDF is used to specify the probability of
    the random variable falling within a particular range of values, as
    opposed to taking on any one value. This probability is given by the
    integral of this variable's PDF over that range—that is, it is given
    by the area under the density function but above the horizontal axis
    and between the lowest and greatest values of the range

    The cumulative distribution function (CDF) of a real-valued random
    variable X, or just distribution function of X , evaluated at b, is the
    probability that X will take a value less than or
    equal to b, so the co domain of CDF is the range of real numbers in [0,1].

    The relation between PDF and CDF:
    Let us suppose we have a function f: R -> R
    This function is evaluated on real numbers. Let's say that we do not know
    this function, but we can sample from its domain and co domain.
    The p(x) = y is the probability of x happening when y is observed, or
    given x we observe y. x is from domain y is from codomain.
    PDF of p(x) is the probability that the value falling into a range.
    So we ask the question what is the probability of x falling into range of
    1 - 8, instead of saying the probability of x happening when 2 is observed
    etc. The PDF gives us how function f behaves in a given range.
    CDF describes the limits of the behaviour of the function.
    Ex. PDF p(x; 0,2) = x / 2 then CDF p(x) = {0: x < 0, x/2: 0<x<2, 1: x>2}

    PDF p(x) in range [a-b]
    PDF p(x) = Pr[a<x<b] = \int_a^b f_X(x)dx
    CDF p(x) = F_X(x) = \int_{-\infty}^x f_X(u)du
    f_X = \lim_{h \to 0} \frac{F_X(x+h) - F_X(x)}{h}
    f_X is the density function. PDF gives the probability of the density
    function

    f_X is the density function. When its value is big, it means the value of
    random variable X is more likely to occur as the value provided by the
    density function.  the value of the PDF at two different samples can be
    used to infer, in any particular draw of the random variable, how much
    more likely it is that the random variable would equal one sample compared
    to the other sample.

    Now let's suppose that we can obtain the CDF of a random variable X.
    We want to generate values that conform to CDF of X. How do we do it ?
    Let's call CDF of X, F_X. The signature of F_X is F_X: R \to {x | 0<x<1 ^
    x \in R} If F_X(X) = Y has a uniform distribution in
    range [0, 1], then we can simply generate a number with a uniform
    distribution in range [0,1] and apply the inverse of F_X to obtain a value
    for X that conforms to CDF.

    Notice that the CDF of X and PDF of X are related. CDF of a sample "b"
    taken from the domain of X, is simply PDF of X evaluated for sample "b" in
    range [-\infty, b]. This makes it possible to define PDF of X using CDF
    using the following relation:
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
