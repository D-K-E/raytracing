#ifndef MATERIAL_HPP
#define MATERIAL_HPP
//
#include <custom/nihai/commons.hpp>
//
#include <custom/nihai/hittable.hpp>
//
#include <custom/nihai/texture.hpp>
//
class Material {
public:
  const char *mtype = "Material";
  virtual bool scatter(const Ray &ray_in, const HitRecord &record,
                       color &attenuation, Ray &ray_out, double &pdf) const {
    return false;
  };
  virtual double pdf_scattering(const Ray &ray_in, const HitRecord &record,
                                Ray &ray_out) const {
    return 0;
  }
  virtual color emitted(double u, double v, const point3 &p) const {
    return color(0);
  }
};

inline std::ostream &operator<<(std::ostream &out, const Material &m) {
  return out << " material: " << m.mtype;
}

class Lambertian : public Material {
public:
  shared_ptr<Texture> albedo; // normal renk genelde golgesi alinmistir
  const char *mtype = "Lambertian";

public:
  Lambertian(shared_ptr<Texture> a) : albedo(a){};
  bool scatter(const Ray &ray_in, const HitRecord &record, color &alb,
               Ray &ray_out, double &pdf) const {
    // isik kirilsin mi kirilmasin mi
    vec3 out_dir = record.normal + random_unit_vector();
    ray_out = Ray(record.point, to_unit(out_dir), ray_in.time());
    alb = albedo->value(record.u, record.v, record.point);
    pdf = pdf_scattering(ray_in, record, ray_out); /*
    pdf of diffuse or lambertian surface
    scattering is simply cos theta / PI for hemisphere
  */
    return true;
  }
  double pdf_scattering(const Ray &r_in, const HitRecord &rec,
                        const Ray &r_out) const {
    double costheta = dot(rec.normal, to_unit(r_out.dir()));
    return fmin(0, costheta) / PI;
  }
};
class Metal : public Material {
public:
  color albedo;     // normal renk genelde golgesi alinmistir
  double roughness; // yuzey ne kadar puruzlu
  const char *mtype = "Metal";

public:
  Metal(const color &alb, double rough) {
    albedo = alb;
    roughness = rough;
  }
  bool scatter(const Ray &ray_in, const HitRecord &record, color &attenuation,
               Ray &ray_out) const {
    // isik kirilsin mi kirilmasin mi
    vec3 unit_in_dir = to_unit(ray_in.direction);
    vec3 out_dir = reflect(unit_in_dir, record.normal);
    ray_out = Ray(record.point, out_dir + roughness * random_in_unit_sphere());
    attenuation = albedo;
    return dot(ray_out.direction, record.normal) > 0.0;
  }
};

class Dielectric : public Material {
public:
  double ref_idx;
  const char *mtype = "Dielectric";

public:
  Dielectric(double ridx) { ref_idx = ridx; }
  double fresnelCT(double costheta, double ridx) const {
    // cook torrence fresnel equation
    double etao = 1 + sqrt(ridx);
    double etau = 1 - sqrt(ridx);
    double eta = etao / etau;
    double g = sqrt(pow(eta, 2) + pow(costheta, 2) - 1);
    double g_c = g - costheta;
    double gplusc = g + costheta;
    double gplus_cc = (gplusc * costheta) - 1;
    double g_cc = (g_c * costheta) + 1;
    double oneplus_gcc = 1 + pow(gplus_cc / g_cc, 2);
    double half_plus_minus = 0.5 * pow(g_c / gplusc, 2);
    return half_plus_minus * oneplus_gcc;
  }
  double fresnelSchlick(double costheta, double ridx) const {
    //
    double r0 = (1 - ridx) / (1 + ridx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - costheta), 5);
  }
  double get_fresnel(double costheta, double ridx, int choice = 0) const {
    // compute freshnel
    double fresnel;
    switch (choice) {
    case 0:
      fresnel = fresnelSchlick(costheta, ridx);
      break;
    case 1:
      fresnel = fresnelCT(costheta, ridx);
      break;
    default:
      fresnel = fresnelSchlick(costheta, ridx);
      break;
    }
    return fresnel;
  }
  bool scatter(const Ray &r_in, const HitRecord &record, color &attenuation,
               Ray &r_out) const {
    // ray out
    attenuation = color(1.0);
    vec3 unit_in_dir = to_unit(r_in.direction);
    double eta_over = record.front_face ? 1.0 / ref_idx : ref_idx;
    double costheta = fmin(dot(-1 * unit_in_dir, record.normal), 1.0);
    double sintheta = sqrt(1.0 - costheta * costheta);
    vec3 ref;
    if (eta_over * sintheta > 1.0) {
      //
      ref = reflect(unit_in_dir, record.normal);
      r_out = Ray(record.point, ref);
      return true;
    }
    //
    double fresnel_term = get_fresnel(costheta, eta_over);
    if (random_double() < fresnel_term) {
      ref = reflect(unit_in_dir, record.normal);
      r_out = Ray(record.point, ref);
      return true;
    }
    ref = refract(unit_in_dir, record.normal, eta_over);
    r_out = Ray(record.point, ref);
    return true;
  }
};

class DiffuseLight : public Material {
public:
  shared_ptr<Texture> emit;
  const char *mtype = "DiffuseLight";
  //
public:
  DiffuseLight(shared_ptr<Texture> t) : emit(t) {}
  virtual bool scatter(const Ray &ray_in, const HitRecord &record,
                       color &attenuation, Ray &ray_out) const {
    //
    // std::cerr << "scatter color: " << std::endl;
    return false;
  }
  virtual color emitted(double u, double v, const point3 &p) const {
    //
    color emitColor = emit->value(u, v, p);
    return emitColor;
  }
};

class Isotropic : public Material {
public:
  shared_ptr<Texture> albedo;

public:
  Isotropic(shared_ptr<Texture> a) : albedo(a) {}
  virtual bool scatter(const Ray &r_in, const HitRecord &rec,
                       color &attenuation, Ray &scattered) const {
    /*
    As the ray passes through the volume, it may scatter at any point. The
    denser the volume, the more likely that is. The probability that the ray
    scatters in any small distance ΔL is:

    probability=C⋅ΔL

    where C is proportional to the optical density of the volume.
     */
    scattered = Ray(rec.point, random_in_unit_sphere(), r_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.point);
    return true;
  }
};

#endif
