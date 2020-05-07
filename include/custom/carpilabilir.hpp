// carpilabilir
#ifndef CARPILABILIR_HPP
#define CARPILABILIR_HPP

#include <custom/ray.hpp>
#include <custom/vec3.hpp>
#include <glm/glm.hpp>

struct CarpmaKaydi {
  nokta3 nokta;
  vek3 dikme;
  double mesafe;
  bool on_taraf;
  inline void tarafa_dik_koy(const Ray &isin, const vek3 &dis_dikme) {
    //
    on_taraf = glm::dot(isin.yonu(), dis_dikme) < 0;
    dikme = on_taraf ? dis_dikme : -1 * dis_dikme;
  }
};
class Carpilabilen {
public:
  virtual bool carpti(const Ray &isin, double min_mesafe, double max_mesafe,
                      CarpmaKaydi &kayit) const = 0;
};

#endif
