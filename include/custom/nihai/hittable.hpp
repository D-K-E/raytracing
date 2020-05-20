// carpilabilir
#ifndef HITTABLE_HPP
#define HITTABLE_HPP
//
#include <cmath>
#include <custom/nihai/aabb.hpp>
#include <custom/nihai/commons.hpp>
//
class Material;

struct HitRecord {
  point3 point;
  vec3 normal;
  shared_ptr<Material> mat_ptr;
  double dist;
  bool front_face;
  inline void set_face_normal(const Ray &r, const vec3 &out_normal) {
    // gelen dikme on yuze mi arka yuze mi
    front_face = dot(r.direction, out_normal) < 0;
    normal = front_face ? out_normal : -1 * out_normal;
  }
  double u, v;
};
/* --------------------- HitRecord utils -----------------*/
inline std::ostream &operator<<(std::ostream &out, const HitRecord &h) {
  // yazim
  return out << "point: " << h.point << " normal: " << h.normal
             << " dist: " << h.dist << " front: " << h.front_face
             << " u: " << h.u << " v: " << h.v << " matptr: " << h.mat_ptr;
}

/* --------------------- HitRecord utils end -----------------*/

class Hittable {
public:
  virtual bool hit(const Ray &r, double dist_min, double dist_max,
                   HitRecord &rec) const = 0;
  virtual bool bounding_box(double t0, double t1, Aabb &output_bbox) const = 0;
  virtual double pdf_value(const point3 &orig, const vec3 &v) const {
    return 0.0;
  }
  virtual vec3 random(const vec3 &orig) const {
    //
    return vec3(1, 0, 0);
  }
};

class Translate : public Hittable {
  //
public:
  shared_ptr<Hittable> ptr;
  vec3 offset;

public:
  Translate(shared_ptr<Hittable> p, const vec3 &displacement)
      : ptr(p), offset(displacement) {}

  virtual bool hit(const Ray &r, double t_min, double t_max,
                   HitRecord &rec) const {
    // hit method check if the object is hit in offset
    Ray moved_r(r.orig() - offset, r.dir(), r.time());
    if (ptr->hit(moved_r, t_min, t_max, rec) == false) {
      return false;
    }
    rec.point += offset;
    rec.set_face_normal(moved_r, rec.normal);
    return true;
  }
  virtual bool bounding_box(double t0, double t1, Aabb &output_box) const {
    // bounding box move with offset
    if (ptr->bounding_box(t0, t1, output_box) == false) {
      return false;
    }
    output_box = Aabb(output_box.min() + offset, output_box.max() + offset);
    return true;
  }
};

class Rotator : public Hittable {
public:
  vec3 rotation_axis;
  double angle_radian;
  shared_ptr<Hittable> ptr;
  bool hasbox;
  Aabb bbox;

public:
  Rotator(shared_ptr<Hittable> _ptr, double angle_degree, const vec3 &axis)
      : ptr(_ptr), rotation_axis(axis),
        angle_radian(degree_to_radian(angle_degree)) {
    //
    ptr->bounding_box(0, 1, bbox);
    point3 pmin(INF);
    point3 pmax(-INF);
    int arr[2] = {0, 1};
    for (const int i : arr) {
      for (const int j : arr) {
        for (const int k : arr) {

          auto x = i * bbox.max().x + (1 - i) * bbox.min().x;
          auto y = j * bbox.max().y + (1 - j) * bbox.min().y;
          auto z = k * bbox.max().z + (1 - k) * bbox.min().z;
          vec3 npos = rotate(vec3(x, y, z));

          int fs[] = {0, 1, 2};

          for (const int f : fs) {
            //
            pmin[f] = fmin(pmin[f], npos[f]);
            pmax[f] = fmax(pmax[f], npos[f]);
          }
        }
      }
    }
    bbox = Aabb(pmin, pmax);
  };

  virtual bool hit(const Ray &r, double tmin, double tmax,
                   HitRecord &rec) const {
    //
    auto origin = r.orig();
    auto dir = r.dir();
    auto norigin = rotate(origin);
    auto ndir = rotate(dir);

    Ray rotated_r(norigin, ndir, r.time());
    if (ptr->hit(rotated_r, tmin, tmax, rec) == false) {
      //
      return false;
    }
    vec3 p = rec.point;
    vec3 n = rec.normal;
    auto rp = rotate(p);
    auto rn = rotate(n);
    rec.point = rp;
    rec.set_face_normal(rotated_r, rn);
    return true;
  }
  virtual bool bounding_box(double t0, double t1, Aabb &output_box) const {
    output_box = bbox;
    return hasbox;
  }
  vec3 rotate(const vec3 &pos) const {
    /*
     rotate position using rodriguez formula:
        p_{rot} = p \cos(\theta) + (k \times p) \sin(\theta) +
                  k (k \cdot p)(1 - \cos(\theta))
     */
    double costheta = cos(angle_radian);
    double sintheta = sin(angle_radian);
    vec3 firstTerm = pos * costheta;
    vec3 secondTerm = cross(rotation_axis, pos) * sintheta;
    vec3 thirdTerm = rotation_axis * dot(rotation_axis, pos) * (1 - costheta);
    return firstTerm + secondTerm + thirdTerm;
  }
};

class RotateY : public Rotator {

public:
  RotateY(shared_ptr<Hittable> _ptr, double angle_degree)
      : Rotator(_ptr, angle_degree, vec3(0, 1, 0)) {}
};
class RotateX : public Rotator {

public:
  RotateX(shared_ptr<Hittable> _ptr, double angle_degree)
      : Rotator(_ptr, angle_degree, vec3(1, 0, 0)) {}
};
class RotateZ : public Rotator {

public:
  RotateZ(shared_ptr<Hittable> _ptr, double angle_degree)
      : Rotator(_ptr, angle_degree, vec3(0, 0, 1)) {}
};

#endif
