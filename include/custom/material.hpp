#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include <custom/commons.hpp>
#include <custom/hittable.hpp>

class Material {
public:
  virtual bool scatter(const Ray &ray_in, const HitRecord &record,
                       color &attenuation, Ray &ray_out) const = 0;
};

#endif
