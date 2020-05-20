#ifndef PDF_HPP
#define PDF_HPP
#include <custom/nihai/commons.hpp>
#include <custom/nihai/hittable.hpp>
#include <custom/nihai/onb.hpp>

class Pdf {
public:
  virtual ~Pdf() {}

  virtual double value(const vec3 &direction) const = 0;
  virtual vec3 generate() const = 0;
};

inline vec3 random_cosine_dir() {
  // random cosine direction
  double r1 = random_double();
  double r2 = random_double();
  auto z = sqrt(1 - r2);

  auto phi = 2 * PI * r1;
  auto x = cos(phi) * sqrt(r2);
  auto y = sin(phi) * sqrt(r2);

  return vec3(x, y, z);
}
inline vec3 random_to_sphere(double radius, double dist_sqr) {
  // produce random point given radius dist sqr
  auto r1 = random_double();
  auto r2 = random_double();
  auto z = 1 + r2 * (sqrt(1 - radius * radius / dist_sqr));

  auto phi = 2 * PI * r1;
  auto x = cos(phi) * sqrt(1 - z * z);
  auto y = sin(phi) * sqrt(1 - z * z);

  return vec3(x, y, z);
}

class CosinePdf : public Pdf {
  //
public:
  CosinePdf(const vec3 &w) { uvw.build_from_w(w); }

  virtual double value(const vec3 &dir) const {
    //
    double cosine = dot(to_unit(dir), uvw.w());
    return (cosine <= 0) ? 0 : cosine / PI;
  }
  virtual vec3 generate() const {
    //
    return uvw.local(random_cosine_dir());
  }

public:
  Onb uvw;
};

class HittablePdf : public Pdf {
  //
public:
  HittablePdf(shared_ptr<Hittable> p, const point3 &origin)
      : ptr(p), o(origin) {}

  virtual double value(const vec3 &direction) const {
    //
    return ptr->pdf_value(o, direction);
  }
  virtual vec3 generate() const {
    //
    return ptr->random(o);
  }

public:
  point3 o;
  shared_ptr<Hittable> ptr;
};

class MixturePdf : public Pdf {
public:
  shared_ptr<Pdf> ptr[2];

public:
  MixturePdf(shared_ptr<Pdf> p0, shared_ptr<Pdf> p1) {
    //
    ptr[0] = p0;
    ptr[1] = p1;
  }
  virtual double value(const vec3 &dir) const {
    return 0.5 * ptr[0]->value(dir) + 0.5 * ptr[1]->value(dir);
  }
  virtual vec3 generate() const {
    //
    // if there were 3 pdf mixtures than it would be
    // 0 < x < 0.3 < y < 0.6 < z < 1
    if (random_double() < 0.5) {
      return ptr[0]->generate();
    } else {
      return ptr[1]->generate();
    }
  }
};

#endif
