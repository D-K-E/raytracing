#ifndef ONB_HPP
#define ONB_HPP
#include <custom/nihai/commons.hpp>

// checked

class Onb {
  // orthonormal base
  /*
    What we need to is generate random directions with a set distribution
    relative to n. We don’t need an origin because a direction is relative
    to no specified origin. We do need two cotangent vectors that are
    mutually perpendicular to n and to each other.

    Some models will come with one or more cotangent vectors. If our model
    has only one cotangent vector, then the process of making an ONB is a
    nontrivial one. Suppose we have any vector a that is of nonzero length
    and not parallel to n. We can get vectors s and t perpendicular to n
    by using the property of the cross product that c×d is perpendicular
    to both c and d:
    Once we have an ONB of s, t, and n, and we have a random(x,y,z) relative
    to the Z-axis, we can get the vector relative to n as:

Random vector=xs+yt+zn

   */
public:
  vec3 axis[3];

public:
  Onb() {}
  inline vec3 operator[](int i) const { return axis[i]; }
  vec3 u() const { return axis[0]; }
  vec3 v() const { return axis[1]; }
  vec3 w() const { return axis[2]; }
  vec3 local(double a, double b, double c) const {
    return a * u() + b * v() + c * w();
  }
  vec3 local(const vec3 &a) const { return a.x * u() + a.y * v() + a.z * w(); }
  void build_from_w(const vec3 &n) {
    // build axis from z vector
    /*
      This is all well and good, but the catch is that we may not be given
      an a when we load a model, and we don't have an a with our existing
      program. If we went ahead and picked an arbitrary a to use as our
      initial vector we may get an a that is parallel to n. A common method
      is to use an if-statement to determine whether n is a particular axis,
      and if not, use that axis.
     */
    axis[2] = to_unit(n);
    vec3 a = (fabs(w().x) > 0.9) ? vec3(0, 1, 0) : vec3(1, 0, 0);
    axis[1] = to_unit(cross(w(), a));
    axis[0] = cross(w(), v());
  }
};

#endif
