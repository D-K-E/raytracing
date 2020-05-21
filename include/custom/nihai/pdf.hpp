#ifndef PDF_HPP
#define PDF_HPP
#include <custom/nihai/commons.hpp>
#include <custom/nihai/hittable.hpp>
#include <custom/nihai/onb.hpp>

// checked

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

  double phi = 2 * PI * r1;
  double x = cos(phi) * sqrt(r2);
  double y = sin(phi) * sqrt(r2);

  return vec3(x, y, z);
}
inline vec3 random_to_sphere(double radius, double dist_sqr) {
  // produce random point given radius dist sqr
  /*

   */
  double r1 = random_double();
  double r2 = random_double();
  double z = 1 + r2 * (sqrt(1 - radius * radius / dist_sqr) - 1);

  double phi = 2 * PI * r1;
  double sqz = sqrt(1 - z * z);
  double x = cos(phi) * sqz;
  double y = sin(phi) * sqz;

  return vec3(x, y, z);
}

inline vec3 random_to_sphere_v1() {
  // pick point in random sphere
  double x1 = random_double(-1, 1);
  double x2 = random_double(-1, 1);
  while ((x1 * x1 + x2 * x2) >= 1) {
    x1 = random_double(-1, 1);
    x2 = random_double(-1, 1);
  }
  double x = 2 * x1 * sqrt(1 - (x1 * x1) - (x2 * x2));
  double y = 2 * x2 * sqrt(1 - (x1 * x1) - (x2 * x2));
  double z = 1 - 2 * ((x1 * x1) + (x2 * x2));
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
    int ptr_index = random_double() < 0.5 ? 0 : 1;
    return ptr[ptr_index]->generate();
  }
};

#endif
