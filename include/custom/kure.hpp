// Kure objesi
#ifndef KURE_HPP
#define KURE_HPP

#include <cmath>
#include <custom/carpilabilir.hpp>
#include <custom/vec3.hpp>
#include <glm/glm.hpp>

/*---------- yardimcilar -----------------------*/
/*---------- yardimcilar bitti -----------------------*/

class Kure : public Carpilabilen {
public:
  Kure() {}
  Kure(nokta3 merkez, double yariCapi);
  kayit_al(double, aralik, double min_mesafe, double max_mesafe,
           CarpmaKaydi &kayit);
  virtual bool carpti(const Ray &isin, double min_mesafe, double max_mesafe,
                      CarpmaKaydi &kayit) const;

public:
  nokta3 merkez;
  double yariCapi;
};
Kure::Kure(nokta3 merkez, double yariCapi) {
  // 1. constructor
  this->merkez = merkez;
  this->yariCapi = yariCapi;
}
bool Kure::kayit_al(double, aralik, double min_mesafe, double max_mesafe,
                    CarpmaKaydi &kayit) {
  // kaydi olustur
  if (aralik < max_mesafe && aralik > min_mesafe) {
    kayit.mesafe = aralik;
    kayit.nokta = isin.konum(kayit.mesafe);
    vek3 dis_dikme = (kayit.nokta - this->merkez) / this->yariCapi;
    kayit.tarafa_dik_koy(isin, dis_dikme);
    return true;
  }
  return false;
}

bool Kure::carpti(const Ray &isin, double min_mesafe, double max_mesafe,
                  CarpmaKaydi &kayit) const {
  // isin kureye vurdu mu vurmadi mi ?
  // points on sphere ||Point - sphere_center|| - sphere_radius = 0

  // || origin_of_ray + direction_of_ray * magnitude - sphere_center|| -
  // sphere_radius = 0
  // || origin_of_ray + direction_of_ray * magnitude - sphere_center|| =
  // sphere_radius
  // || origin_of_ray + direction_of_ray * magnitude - sphere_center||^2 =
  // sphere_radius^2
  // || o + d * m - c ||^2 = r^2
  // (o+d*m-c) (o+d*m-c) = r^2
  // d^2 * m^2 + 2d(m *(o-c)) + (o-c)^2 = r^2
  // ax^2 + bx + c = 0
  // a = d^2
  // x = m
  // b = 2*(d*(o-c))
  // c = (o-c)^2 - r^2
  // m = -b ± sqrt(b^2 - c)
  //
  // intersect hit on 2 points b^2 - c > 0
  // miss case hit on 0 point b^2 - c < 0
  // tangent hit on 1 point b^2 - c = 0
  vek3 merkezeDogru = isin.basi() - this->merkez;
  double a = glm::dot(isin.yonu(), isin.yonu());
  double yarim_b = glm::dot(merkezeDogru, isin.yonu());
  double c =
      glm::dot(merkezeDogru, merkezeDogru) - this->yariCapi * this->yariCapi;
  double b2_c = yarim_b * yarim_b - a * c;
  if (b2_c > 0) {
    double b2_m_c_koku = std::sqrt(b2_c);
    double aralik = (-1 * yarim_b - b2_m_c_koku) / a;

    if (this->kayit_al(aralik, min_mesafe, max_mesafe, kayit)) {
      return true;
    }
    double aralik = (-1 * yarim_b + b2_m_c_koku) / a;
    if (this->kayit_al(aralik, min_mesafe, max_mesafe, kayit)) {
      return true;
    }
  }
  return false;
}

#endif
