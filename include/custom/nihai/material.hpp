#ifndef MATERIAL_HPP
#define MATERIAL_HPP
//
#include <custom/nihai/commons.hpp>
//
#include <custom/nihai/pdf.hpp>
//
#include <custom/nihai/texture.hpp>
//

double schlick(double cosine, double ref_idx) {
  double r0 = (1 - ref_idx) / (1 + ref_idx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cosine), 5);
}

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

  virtual double pdf_scattering(const Ray &r_in, const HitRecord &rec,
                                const Ray &scattered) const {
    return 0;
  }
};

class Dielectric : public Material {
public:
  Dielectric(double ri) : ref_idx(ri) {}

  virtual bool scatter(const Ray &r_in, const HitRecord &rec,
                       ScatterRecord &srec) const {
    srec.is_specular = true;
    srec.pdf_ptr = nullptr;
    srec.attenuation = color(1.0, 1.0, 1.0);
    double etai_over_etat = (rec.front_face) ? (1.0 / ref_idx) : (ref_idx);

    vec3 unit_direction = to_unit(r_in.dir());
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    if (etai_over_etat * sin_theta > 1.0) {
      vec3 reflected = reflect(unit_direction, rec.normal);
      srec.r_out = Ray(rec.point, reflected, r_in.time());
      return true;
    }

    double reflect_prob = schlick(cos_theta, etai_over_etat);
    if (random_double() < reflect_prob) {
      vec3 reflected = reflect(unit_direction, rec.normal);
      srec.r_out = Ray(rec.point, reflected, r_in.time());
      return true;
    }

    vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
    srec.r_out = Ray(rec.point, refracted, r_in.time());
    return true;
  }

public:
  double ref_idx;
};

class DiffuseLight : public Material {
public:
  DiffuseLight(shared_ptr<Texture> a) : emit(a) {}

  virtual color emitted(const Ray &r_in, const HitRecord &rec, double u,
                        double v, const point3 &p) const {
    if (!rec.front_face)
      return color(0, 0, 0);
    return emit->value(u, v, p);
  }

public:
  shared_ptr<Texture> emit;
};

class Isotropic : public Material {
public:
  Isotropic(shared_ptr<Texture> a) : albedo(a) {}

  virtual bool scatter(const Ray &r_in, const HitRecord &rec,
                       color &attenuation, Ray &scattered) const {
    scattered = Ray(rec.point, random_in_unit_sphere(), r_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.point);
    return true;
  }

public:
  shared_ptr<Texture> albedo;
};

class Lambertian : public Material {
public:
  Lambertian(shared_ptr<Texture> a) : albedo(a) {}

  virtual bool scatter(const Ray &r_in, const HitRecord &rec,
                       ScatterRecord &srec) const {
    srec.is_specular = false;
    srec.attenuation = albedo->value(rec.u, rec.v, rec.point);
    srec.pdf_ptr = make_shared<CosinePdf>(rec.normal);
    return true;
  }

  double pdf_scattering(const Ray &r_in, const HitRecord &rec,
                        const Ray &scattered) const {
    auto cosine = dot(rec.normal, to_unit(scattered.dir()));
    return cosine < 0 ? 0 : cosine / PI;
  }

public:
  shared_ptr<Texture> albedo;
};

class Metal : public Material {
public:
  Metal(const color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

  virtual bool scatter(const Ray &r_in, const HitRecord &rec,
                       ScatterRecord &srec) const {
    vec3 reflected = reflect(to_unit(r_in.dir()), rec.normal);
    srec.r_out =
        Ray(rec.point, reflected + fuzz * random_in_unit_sphere(), r_in.time());
    srec.attenuation = albedo;
    srec.is_specular = true;
    srec.pdf_ptr = nullptr;
    return true;
  }

public:
  color albedo;
  double fuzz;
};

#endif
