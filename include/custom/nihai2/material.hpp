#ifndef MATERIAL_HPP
#define MATERIAL_HPP

//
#include <custom/nihai2/commons.hpp>
//
#include <custom/nihai2/pdf.hpp>
//
#include <custom/nihai2/texture.hpp>
//

struct ScatterRecord {
  Ray r_out;
  bool is_specular;
  color attenuation;
  shared_ptr<Pdf> pdf_ptr;
};

class Material {
public:
  virtual color emitted(const Ray &r_in, const HitRecord &rec, double u,
                        double v, const point3 &p) const {
    return color(0);
  }
  virtual bool scatter(const Ray &r_in, const HitRecord &rec,
                       ScatterRecord &srec) const {
    return false;
  }
  virtual double scattering_pdf(const Ray &r_in, const HitRecord &rec,
                                const Ray &scattered) const {
    return 0;
  }
};
class Dielectric : public Material {
public:
  double ref_idx;

  Dielectric(double r) : ref_idx(r) {}
  bool scatter(const Ray &r_in, const HitRecord &rec,
               ScatterRecord &srec) override {
    srec.is_specular = true;
    srec.pdf_ptr = nullptr;
    srec.attenuation = color(1.0);
    double etai_over_etat = (rec.front_face) ? (1.0 / ref_idx) : (ref_idx);

    vec3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    if (etai_over_etat * sin_theta > 1.0) {
      vec3 reflected = reflect(unit_direction, rec.normal);
      srec.specular_ray = ray(rec.p, reflected, r_in.time());
      return true;
    }

    double reflect_prob = schlick(cos_theta, etai_over_etat);
    if (random_double() < reflect_prob) {
      vec3 reflected = reflect(unit_direction, rec.normal);
      srec.specular_ray = ray(rec.p, reflected, r_in.time());
      return true;
    }

    vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
    srec.specular_ray = ray(rec.p, refracted, r_in.time());
    return true;
  }
};
class DiffuseLight : public Material {
public:
  shared_ptr<Texture> emit;

  DiffuseLight(shared_ptr<Texture> a) : emit(a) {}
  color emitted(const Ray &r_in, const HitRecord &rec, double u, double v,
                const point3 &p) const override {
    if (!rec.front_face)
      return color(0);
    return emit->value(u, v, p);
  }
};
class Isotropic : public Material {
public:
  Isotropic(shared_ptr<Texture> a) : albedo(a) {}

  bool scatter(const Ray &r_in, const HitRecord &rec,
               ScatterRecord &srec) const override {
    srec.r_out = Ray(rec.p, random_in_unit_sphere(), r_in.time());
    srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
  }

public:
  shared_ptr<Texture> albedo;
};
class Lambertian : public Material {
public:
  Lambertian(shared_ptr<Texture> a) : albedo(a) {}

  bool scatter(const Ray &r_in, const HitRecord &rec,
               ScatterRecord &srec) const override {
    srec.is_specular = false;
    srec.attenuation = albedo->value(rec.u, rec.v, rec.p);
    srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);
    return true;
  }

  double scattering_pdf(const Ray &r_in, const HitRecord &rec,
                        const Ray &scattered) const override {
    auto cosine = dot(rec.normal, unit_vector(scattered.direction()));
    return cosine < 0 ? 0 : cosine / pi;
  }

public:
  shared_ptr<Texture> albedo;
};
class Metal : public Material {
public:
  Metal(const color &a, double f) : albedo(a), roughness(f < 1 ? f : 1) {}

  bool scatter(const Ray &r_in, const HitRecord &rec,
               ScatterRecord &srec) const override {
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    srec.r_out = Ray(rec.p, reflected + roughness * random_in_unit_sphere(),
                     r_in.time());
    srec.attenuation = albedo;
    srec.is_specular = true;
    srec.pdf_ptr = nullptr;
    return true;
  }

public:
  color albedo;
  double roughness;
};

#endif
