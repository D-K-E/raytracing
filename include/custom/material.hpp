#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include <custom/commons.hpp>
#include <custom/hittable.hpp>

class Material {
public:
  virtual bool scatter(const Ray &ray_in, const HitRecord &record,
                       color &attenuation, Ray &ray_out) const = 0;
};

class Lambertian : public Material {
public:
  color albedo; // normal renk genelde golgesi alinmistir

public:
  Lambertian(const color &alb) { albedo = alb; }
  bool scatter(const Ray &ray_in, const HitRecord &record, color &attenuation,
               Ray &ray_out) const {
    // isik kirilsin mi kirilmasin mi
    vec3 out_dir = record.normal + random_unit_vector();
    ray_out = Ray(record.point, out_dir);
    attenuation = albedo;
    return true;
  }
};
class Metal : public Material {
public:
  color albedo; // normal renk genelde golgesi alinmistir

public:
  Metal(const color &alb) { albedo = alb; }
  bool scatter(const Ray &ray_in, const HitRecord &record, color &attenuation,
               Ray &ray_out) const {
    // isik kirilsin mi kirilmasin mi
    vec3 unit_in_dir = to_unit(ray_in.direction);
    vec3 out_dir = reflect(unit_in_dir, record.normal);
    ray_out = Ray(record.point, out_dir);
    attenuation = albedo;
    return dot(ray_out.direction, record.normal) > 0.0;
  }
};

#endif
