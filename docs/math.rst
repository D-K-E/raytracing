##########
Math Notes
##########

A random variable is a function whose domain is the sample space and whose
codomain is a measurable set. In many cases the codomain is R. An expected
value of a random variable X, ie function X,is :math:`\sum_{s \in S} p(s)X(s)`
where S is the sample space. It reads as sum of the probability of sample s
given and the value of random variable X evaluated for sample s.


Importance sampling is a general technique for estimating properties of a
particular distribution, while only having samples generated from a different
distribution than the distribution of interest

In importance sampling we assume the following scenario: I have a function f:
[a, b] → R is an integrable real-valued function. X is a random variable whose
domain is the interval [a, b], with distribution g. f(X)/g(X) is a random
variable whose expected value, or mean value (they imply the same thing),
E[f(x)] = \int_a^b f(x). Meaning that the expected value of f(x) coincides
with that of f(x)/g(x) when we plug f(x)/g(x) to the evaluation of the
expected value of the random variable X. Now if we can find a constant ratio
between them something like f(x)/g(x) = some constant C, our estimation would
be unbiased and we would live happily here after.  But for us to know find
that constant we would need to know the f(x), that is the answer we are trying
to solve. However, if we can choose a g(x) that gets larger when f(x) gets
larger and gets lower when f(x) gets lower, we would decrease the variance of
our estimations.

How do we choose g ? You simply choose a probability density function (PDF)
that is evaluated as non zero within the interval of f.

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


=======================
Random point on Sphere
=======================
https://mathworld.wolfram.com/SpherePointPicking.html

To pick a random point on the surface of a unit sphere, it is incorrect to
select spherical coordinates theta and phi from uniform distributions theta in
[0,2pi) and phi in [0,pi], since the area element :math:`d\Omega=\sin(\phi) d
\theta d\phi` is a function of phi, and hence points picked in this way will
be "bunched" near the poles (left figure above).

To obtain points such that any small area on the sphere is expected to contain
the same number of points (right figure above), choose U and V to be random
variates on (0,1). 

- :math:`\theta = 2* \pi * u`	
- :math:`phi = \cos^(-1)(2v-1)`

gives the spherical coordinates for a set of points which are uniformly
distributed over S^2. This works since the differential element of solid angle
is given by 
