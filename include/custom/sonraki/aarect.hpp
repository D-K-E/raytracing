#ifndef AARECT_HPP
#define AARECT_HPP
#include <custom/sonraki/aabb.hpp>
#include <custom/sonraki/commons.hpp>
#include <custom/sonraki/hittable.hpp>
#include <custom/sonraki/material.hpp>

struct AxisInfo {
  unsigned int aligned1;
  unsigned int aligned2;
  unsigned int notAligned;
};

class AaRect : public Hittable {
protected:
  vec3 axis_normal;
  double a0, a1, // aligned1
      b0, b1;    // aligned2
  AxisInfo ax;

public:
  double k;
  shared_ptr<Material> mat_ptr;

public:
  AaRect() {}
  AaRect(double a_0, double a_1, double b_0, double b_1, double _k,
         shared_ptr<Material> mat, vec3 anormal)
      : a0(a_0), a1(a_1), b0(b_0), b1(b_1), k(_k), mat_ptr(mat),
        axis_normal(anormal) {
    if (axis_normal.z == 1.0) {
      // xy rect
      ax.aligned1 = 0;
      ax.aligned2 = 1;
      ax.notAligned = 2;
    } else if (axis_normal.x == 1.0) {
      // yz rect
      ax.aligned1 = 1;
      ax.aligned2 = 2;
      ax.notAligned = 0;
    } else if (axis_normal.y == 1.0) {
      // xz rect
      ax.aligned1 = 0;
      ax.aligned2 = 2;
      ax.notAligned = 1;
    }
  }
  bool hit(const Ray &r, double t0, double t1, HitRecord &rec) const override {
    //
    //  point of intersection satisfies
    //  both P = O + D*m Point = Origin + Direction * magnitude
    //  and
    //  x0 < x_i < x1 y0 < y_i < y1 y0,y1 and x0,x1 being the limits of
    //  rectangle
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
  bool bounding_box(double t0, double t1, Aabb &output_box) const override {
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
};

class XYRect : public AaRect {
public:
  double x0, x1, y0, y1;

public:
  XYRect() {}
  XYRect(double _x0, double _x1, double _y0, double _y1, double _k,
         shared_ptr<Material> mat)
      : AaRect(_x0, _x1, _y0, _y1, _k, mat, vec3(0, 0, 1)), x0(_x0), x1(_x1),
        y0(_y0), y1(_y1) {}
};
class XZRect : public AaRect {

public:
  XZRect() {}
  XZRect(double _x0, double _x1, double _z0, double _z1, double _k,
         shared_ptr<Material> mat)
      : AaRect(_x0, _x1, _z0, _z1, _k, mat, vec3(0, 1, 0)) {}
};
class YZRect : public AaRect {

public:
  YZRect() {}
  YZRect(double _y0, double _y1, double _z0, double _z1, double _k,
         shared_ptr<Material> mat)
      : AaRect(_y0, _y1, _z0, _z1, _k, mat, vec3(1, 0, 0)) {}
};

/*
class XYRect : public Hittable {
public:
  double x0, x1, y0, y1, k;
  vec3 axis_normal = vec3(0, 0, 1);
  shared_ptr<Material> mat_ptr;

public:
  XYRect() {}
  XYRect(double _x0, double _x1, double _y0, double _y1, double _k,
         shared_ptr<Material> mat)
      : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mat_ptr(mat) {}

  bool hit(const Ray &r, double t0, double t1, HitRecord &rec) const override;

  bool bounding_box(double t0, double t1, Aabb &output_box) const override;
};
class XZRect : public Hittable {

public:
  double x0, x1, z0, z1, k;
  vec3 axis_normal = vec3(0, 1, 0);
  shared_ptr<Material> mat_ptr;

public:
  XZRect() {}
  XZRect(double _x0, double _x1, double _z0, double _z1, double _k,
         shared_ptr<Material> mat)
      : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mat_ptr(mat) {}
  bool hit(const Ray &r, double t0, double t1, HitRecord &rec) const override;
  bool bounding_box(double t0, double t1, Aabb &output_box) const override;
};
class YZRect : public Hittable {
public:
  double y0, y1, z0, z1, k;
  vec3 axis_normal = vec3(1, 0, 0);
  shared_ptr<Material> mat_ptr;

public:
  YZRect() {}
  YZRect(double _y0, double _y1, double _z0, double _z1, double _k,
         shared_ptr<Material> mat)
      : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mat_ptr(mat) {}
  bool hit(const Ray &r, double t0, double t1, HitRecord &rec) const override;
  bool bounding_box(double t0, double t1, Aabb &output_box) const override;
};

bool XYRect::hit(const Ray &r, double t0, double t1, HitRecord &rec) const {
  // hit function
  auto t = (k - r.orig().z) / r.dir().z;
  if (t < t0 || t > t1)
    return false;
  auto x = r.orig().x + t * r.dir().x;
  auto y = r.orig().y + t * r.dir().y;
  if (x < x0 || x > x1 || y < y0 || y > y1)
    return false;
  rec.u = (x - x0) / (x1 - x0);
  rec.v = (y - y0) / (y1 - y0);
  rec.dist = t;
  auto outward_normal = axis_normal;
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mat_ptr;
  rec.point = r.at(t);
  return true;
}
bool XZRect::hit(const Ray &r, double t0, double t1, HitRecord &rec) const {
  // hit function
  auto t = (k - r.orig().y) / r.dir().y;
  if (t < t0 || t > t1)
    return false;
  auto x = r.orig().x + t * r.dir().x;
  auto z = r.orig().z + t * r.dir().z;
  if (x < x0 || x > x1 || z < z0 || z > z1)
    return false;
  rec.u = (x - x0) / (x1 - x0);
  rec.v = (z - z0) / (z1 - z0);
  rec.dist = t;
  auto outward_normal = axis_normal;
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mat_ptr;
  rec.point = r.at(t);
  return true;
}
bool YZRect::hit(const Ray &r, double t0, double t1, HitRecord &rec) const {
  // hit function
  auto t = (k - r.orig().x) / r.dir().x;
  if (t < t0 || t > t1)
    return false;
  auto y = r.orig().y + t * r.dir().y;
  auto z = r.orig().z + t * r.dir().z;
  if (y < y0 || y > y1 || z < z0 || z > z1)
    return false;
  rec.u = (y - y0) / (y1 - y0);
  rec.v = (z - z0) / (z1 - z0);
  rec.dist = t;
  auto outward_normal = axis_normal;
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mat_ptr;
  rec.point = r.at(t);
  return true;
}

bool XYRect::bounding_box(double t0, double t1, Aabb &output_box) const {
  //
  output_box = Aabb(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
  return true;
}
bool XZRect::bounding_box(double t0, double t1, Aabb &output_box) const {
  //
  output_box = Aabb(point3(x0, k - 0.0001, z0), point3(x1, k + 0.0001, z1));
  return true;
}
bool YZRect::bounding_box(double t0, double t1, Aabb &output_box) const {
  //
  output_box = Aabb(point3(k - 0.0001, y0, z0), point3(k + 0.0001, y1, z1));
  return true;
}
*/

#endif
