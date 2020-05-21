#ifndef AARECT_HPP
#define AARECT_HPP
#include <custom/nihai/aabb.hpp>
#include <custom/nihai/commons.hpp>
#include <custom/nihai/hittable.hpp>
#include <custom/nihai/material.hpp>

struct AxisInfo {
  double aligned1;
  double aligned2;
  double notAligned;
};
inline std::ostream &operator<<(std::ostream &out, const AxisInfo &a) {
  // yazim
  return out << "aligned1: " << a.aligned1 << " aligned2: " << a.aligned2
             << " not aligned: " << a.notAligned;
}

class AaRect : public Hittable {
public:
  vec3 axis_normal;
  double a0, a1, // aligned1
      b0, b1;    // aligned2
  AxisInfo ax;

public:
  double k;
  shared_ptr<Material> mat_ptr;
  const char *mtype = "Aarect";

public:
  AaRect() {}
  AaRect(double a_0, double a_1, double b_0, double b_1, double _k,
         shared_ptr<Material> mat, vec3 anormal)
      : a0(a_0), a1(a_1), b0(b_0), b1(b_1), k(_k), mat_ptr(mat),
        axis_normal(anormal) {
    if (anormal.z == 1.0) {
      ax.aligned1 = 0;
      ax.aligned2 = 1;
      ax.notAligned = 2;
    } else if (anormal.x == 1.0) {
      ax.aligned1 = 1;
      ax.aligned2 = 2;
      ax.notAligned = 0;
    } else if (anormal.y == 1.0) {
      ax.aligned1 = 0;
      ax.aligned2 = 2;
      ax.notAligned = 1;
    }
  }
  virtual bool hit(const Ray &r, double t0, double t1, HitRecord &rec) const {
    /*
       point of intersection satisfies
       both P = O + D*m Point = Origin + Direction * magnitude
       and
       x0 < x_i < x1 y0 < y_i < y1 y0,y1 and x0,x1 being the limits of
       rectangle
     */
    auto t = (k - r.orig()[ax.notAligned]) / r.dir()[ax.notAligned];
    if (t < t0 || t > t1)
      return false;
    double a = r.orig()[ax.aligned1] + t * r.dir()[ax.aligned1];
    double b = r.orig()[ax.aligned2] + t * r.dir()[ax.aligned2];
    bool c1 = a0 < a and a < a1;
    bool c2 = b0 < b and b < b1;
    if ((c1 and c2) == false) {
      return false;
    }
    rec.u = (a - a0) / (a1 - a0);
    rec.v = (b - b0) / (b1 - b0);
    rec.dist = t;
    vec3 outward_normal = axis_normal;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    rec.point = r.at(t);
    return true;
  }
  virtual bool bounding_box(double t0, double t1, Aabb &output_box) const {
    // The bounding box must have non-zero width in each dimension, so pad the Z
    // dimension a small amount.
    point3 p1, p2;
    // choose points with axis
    if (ax.notAligned == 2) {
      p1 = point3(a0, b0, k - 0.0001);
      p2 = point3(a1, b1, k + 0.0001);
    } else if (ax.notAligned == 1) {
      p1 = point3(a0, k - 0.0001, b0);
      p2 = point3(a1, k + 0.0001, b1);
    } else if (ax.notAligned == 0) {
      p1 = point3(k - 0.0001, a0, b0);
      p2 = point3(k + 0.0001, a1, b1);
    }
    output_box = Aabb(p1, p2);
    return true;
  }
  virtual double pdf_value(const point3 &orig, const vec3 &v) const {
    HitRecord rec;
    if (this->hit(Ray(orig, v), 0.001, INF, rec) == false) {
      //
      return 0;
    }
    auto area = fabs(a0 - a1) * fabs(b0 - b1);
    double dist_sqr = rec.dist * rec.dist * length_sqr(v);
    double cosine = fabs(dot(v, rec.normal) / length(v));
    return dist_sqr / (cosine * area);
  }
  virtual vec3 random(const point3 &orig) const {
    //
    point3 p1;
    auto arnd = random_double(fmin(a0, a1), fmax(a0, a1));
    auto brnd = random_double(fmin(b0, b1), fmax(b0, b1));
    // choose points with axis
    if (ax.notAligned == 2) {
      // normal vec3(0,0,1)
      p1 = point3(arnd, brnd, k);
    } else if (ax.notAligned == 1) {
      // normal vec3(0,1,0)
      p1 = point3(arnd, k, brnd);
    } else if (ax.notAligned == 0) {
      // normal vec3(1,0,0)
      p1 = point3(k, arnd, brnd);
    }
    return p1 - orig;
  }
};
inline std::ostream &operator<<(std::ostream &out, const AaRect &a) {
  // yazim
  return out << "type: " << a.mtype << " a0: " << a.a0 << " a1: " << a.a1
             << " b0: " << a.b0 << " b1: " << a.b1 << " k: " << a.k
             << " axinfo: " << a.ax;
}

class XYRect : public AaRect {
public:
  double x0, x1, y0, y1;
  const char *mtype = "xyrect";

public:
  XYRect() {}
  XYRect(double _x0, double _x1, double _y0, double _y1, double _k,
         shared_ptr<Material> mat)
      : AaRect(_x0, _x1, _y0, _y1, _k, mat, vec3(0, 0, 1)), x0(_x0), x1(_x1),
        y0(_y0), y1(_y1) {}
};
class XZRect : public AaRect {
public:
  double x0, x1, z0, z1;
  const char *mtype = "xzrect";

public:
  XZRect() {}
  XZRect(double _x0, double _x1, double _z0, double _z1, double _k,
         shared_ptr<Material> mat)
      : AaRect(_x0, _x1, _z0, _z1, _k, mat, vec3(0, 1, 0)), x0(_x0), x1(_x1),
        z0(_z0), z1(_z1) {}

  virtual vec3 random(const point3 &origin) const {
    auto random_point = point3(random_double(x0, x1), k, random_double(z0, z1));
    return random_point - origin;
  }
};
class YZRect : public AaRect {
public:
  double y0, y1, z0, z1;
  const char *mtype = "yzrect";

public:
  YZRect() {}
  YZRect(double _y0, double _y1, double _z0, double _z1, double _k,
         shared_ptr<Material> mat)
      : AaRect(_y0, _y1, _z0, _z1, _k, mat, vec3(1, 0, 0)), y0(_y0), y1(_y1),
        z0(_z0), z1(_z1) {}
};

#endif
