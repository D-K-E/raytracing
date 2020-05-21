#ifndef AARECT_HPP
#define AARECT_HPP

#include <custom/nihai/commons.hpp>
#include <custom/nihai/hittable.hpp>

class XYRect : public Hittable {
public:
  XYRect() {}

  XYRect(double _x0, double _x1, double _y0, double _y1, double _k,
         shared_ptr<Material> mat)
      : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat){};

  virtual bool hit(const Ray &r, double t0, double t1, HitRecord &rec) const;

  virtual bool bounding_box(double t0, double t1, Aabb &output_box) const {
    // The bounding box must have non-zero width in each dimension, so pad the Z
    // dimension a small amount.
    output_box = Aabb(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
    return true;
  }

public:
  shared_ptr<Material> mp;
  double x0, x1, y0, y1, k;
};

class XZRect : public Hittable {
public:
  XZRect() {}

  XZRect(double _x0, double _x1, double _z0, double _z1, double _k,
         shared_ptr<Material> mat)
      : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat){};

  virtual bool hit(const Ray &r, double t0, double t1, HitRecord &rec) const;

  virtual bool bounding_box(double t0, double t1, Aabb &output_box) const {
    // The bounding box must have non-zero width in each dimension, so pad the Y
    // dimension a small amount.
    output_box = Aabb(point3(x0, k - 0.0001, z0), point3(x1, k + 0.0001, z1));
    return true;
  }

  virtual double pdf_value(const point3 &origin, const vec3 &v) const {
    HitRecord rec;
    if (!this->hit(Ray(origin, v), 0.001, INF, rec))
      return 0;

    auto area = (x1 - x0) * (z1 - z0);
    auto distance_squared = rec.dist * rec.dist * dot(v, v);
    auto cosine = fabs(dot(v, rec.normal) / v.length());

    return distance_squared / (cosine * area);
  }

  virtual vec3 random(const point3 &origin) const {
    auto random_point = point3(random_double(x0, x1), k, random_double(z0, z1));
    return random_point - origin;
  }

public:
  shared_ptr<Material> mp;
  double x0, x1, z0, z1, k;
};

class YZRect : public Hittable {
public:
  YZRect() {}

  YZRect(double _y0, double _y1, double _z0, double _z1, double _k,
         shared_ptr<Material> mat)
      : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat){};

  virtual bool hit(const Ray &r, double t0, double t1, HitRecord &rec) const;

  virtual bool bounding_box(double t0, double t1, Aabb &output_box) const {
    // The bounding box must have non-zero width in each dimension, so pad the X
    // dimension a small amount.
    output_box = Aabb(point3(k - 0.0001, y0, z0), point3(k + 0.0001, y1, z1));
    return true;
  }

public:
  shared_ptr<Material> mp;
  double y0, y1, z0, z1, k;
};

bool XYRect::hit(const Ray &r, double t0, double t1, HitRecord &rec) const {
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
  auto outward_normal = vec3(0, 0, 1);
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mp;
  rec.point = r.at(t);

  return true;
}

bool XZRect::hit(const Ray &r, double t0, double t1, HitRecord &rec) const {
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
  auto outward_normal = vec3(0, 1, 0);
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mp;
  rec.point = r.at(t);

  return true;
}

bool YZRect::hit(const Ray &r, double t0, double t1, HitRecord &rec) const {
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
  auto outward_normal = vec3(1, 0, 0);
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mp;
  rec.point = r.at(t);

  return true;
}

#endif
