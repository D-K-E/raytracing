#ifndef COSDENSE_HPP
#define COSDENSE_HPP
#include <custom/nihai/commons.hpp>
#include <math.h>

/*
The goal is to create random vector generator, that generates vectors aligned
to a certain axis. This in fact importance sampling applied to vector
generators.

To simplify things, let’s assume the z-axis is the surface normal, and θ is
the angle from the normal.

Now generating a unit vector means generating two random numbers on unit
sphere basically a theta and a phi, (θ,ϕ). A uniform generation would mean
that the range for theta is [0, pi] and for phi [0, 2pi], for theta then we
would have 1 / pi and for phi 1/2pi as pdf of any direction.

To generate a unit vector direction toward (θ,ϕ) we convert to Cartesian
coordinates:

x=cos(ϕ)⋅sin(θ)
y=sin(ϕ)⋅sin(θ)
z=cos(θ)

 */

vec3 random_cos_dir() {
  // random cosinus direction in unit sphere
  auto r1 = random_double();
  auto r2 = random_double();
  double z = sqrt(1 - r2);

  double phi = 2 * PI * r1;
  auto x = cos(phi) * sqrt(r2);
  auto y = sin(phi) * sqrt(r2);
  return vec3(x, y, z);
}

#endif
