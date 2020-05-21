#ifndef PDF_HPP
#define PDF_HPP
//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright
// and related and
// neighboring rights to this software to the public domain worldwide. This
// software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public
// Domain Dedication
// along with this software. If not, see
// <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include <custom/nihai/commons.hpp>

#include <custom/nihai/onb.hpp>

inline vec3 random_cosine_direction() {
  auto r1 = random_double();
  auto r2 = random_double();
  auto z = sqrt(1 - r2);

  auto phi = 2 * PI * r1;
  auto x = cos(phi) * sqrt(r2);
  auto y = sin(phi) * sqrt(r2);

  return vec3(x, y, z);
}

inline vec3 random_to_sphere(double radius, double distance_squared) {
  auto r1 = random_double();
  auto r2 = random_double();
  auto z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);

  auto phi = 2 * PI * r1;
  auto x = cos(phi) * sqrt(1 - z * z);
  auto y = sin(phi) * sqrt(1 - z * z);

  return vec3(x, y, z);
}

class Pdf {
public:
  virtual ~Pdf() {}

  virtual double value(const vec3 &direction) const = 0;
  virtual vec3 generate() const = 0;
};

class CosinePdf : public Pdf {
public:
  CosinePdf(const vec3 &w) { uvw.build_from_w(w); }

  virtual double value(const vec3 &direction) const {
    auto cosine = dot(to_unit(direction), uvw.w());
    return (cosine <= 0) ? 0 : cosine / PI;
  }

  virtual vec3 generate() const { return uvw.local(random_cosine_direction()); }

public:
  Onb uvw;
};

class HittablePdf : public Pdf {
public:
  HittablePdf(shared_ptr<Hittable> p, const point3 &origin)
      : ptr(p), o(origin) {}

  virtual double value(const vec3 &direction) const {
    return ptr->pdf_value(o, direction);
  }

  virtual vec3 generate() const { return ptr->random(o); }

public:
  point3 o;
  shared_ptr<Hittable> ptr;
};

class MixturePdf : public Pdf {
public:
  MixturePdf(shared_ptr<Pdf> p0, shared_ptr<Pdf> p1) {
    p[0] = p0;
    p[1] = p1;
  }

  virtual double value(const vec3 &direction) const {
    return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
  }

  virtual vec3 generate() const {
    if (random_double() < 0.5)
      return p[0]->generate();
    else
      return p[1]->generate();
  }

public:
  shared_ptr<Pdf> p[2];
};

#endif
