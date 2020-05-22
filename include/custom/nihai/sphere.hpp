// Kure objesi
#ifndef SPHERE_HPP
#define SPHERE_HPP

//
#include <custom/nihai/commons.hpp> // vek3, ray
//
#include <custom/nihai/hittable.hpp>
//
#include <custom/nihai/onb.hpp>
//

class Sphere : public Hittable {
public:
  Sphere() {}
  Sphere(point3 cen, double r, shared_ptr<Material> m)
      : center(cen), radius(r), mat_ptr(m){};
  virtual bool hit(const Ray &r, double tmin, double tmax,
                   HitRecord &rec) const;
  virtual bool bounding_box(double t0, double t1, Aabb &output_box) const;
  virtual double pdf_value(const point3 &o, const vec3 &v) const;
  virtual vec3 random(const point3 &o) const;

public:
  point3 center;
  double radius;
  shared_ptr<Material> mat_ptr;
};

double Sphere::pdf_value(const point3 &o, const vec3 &v) const {
  HitRecord rec;
  if (!this->hit(Ray(o, v), 0.001, INF, rec))
    return 0;

  auto cos_theta_max = sqrt(1 - radius * radius / length_sqr(center - o));
  auto solid_angle = 2 * PI * (1 - cos_theta_max);

  return 1 / solid_angle;
}

vec3 Sphere::random(const point3 &o) const {
  vec3 direction = center - o;
  auto distance_squared = length_sqr(direction);
  Onb uvw;
  uvw.build_from_w(direction);
  return uvw.local(random_to_sphere(radius, distance_squared));
}

bool Sphere::bounding_box(double t0, double t1, Aabb &output_box) const {
  output_box = Aabb(center - vec3(radius, radius, radius),
                    center + vec3(radius, radius, radius));
  return true;
}

bool Sphere::hit(const Ray &r, double t_min, double t_max,
                 HitRecord &rec) const {
  vec3 oc = r.orig() - center;
  auto a = length_sqr(r.dir());
  auto half_b = dot(oc, r.dir());
  auto c = length_sqr(oc) - radius * radius;

  auto discriminant = half_b * half_b - a * c;

  if (discriminant > 0) {
    auto root = sqrt(discriminant);

    auto temp = (-half_b - root) / a;
    if (temp < t_max && temp > t_min) {
      rec.dist = temp;
      rec.point = r.at(rec.dist);
      vec3 outward_normal = (rec.point - center) / radius;
      rec.set_face_normal(r, outward_normal);
      get_sphere_uv((rec.point - center) / radius, rec.u, rec.v);
      rec.mat_ptr = mat_ptr;
      return true;
    }

    temp = (-half_b + root) / a;
    if (temp < t_max && temp > t_min) {
      rec.dist = temp;
      rec.point = r.at(rec.dist);
      vec3 outward_normal = (rec.point - center) / radius;
      rec.set_face_normal(r, outward_normal);
      get_sphere_uv((rec.point - center) / radius, rec.u, rec.v);
      rec.mat_ptr = mat_ptr;
      return true;
    }
  }

  return false;
}

#endif
