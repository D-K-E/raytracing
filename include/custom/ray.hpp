#ifndef RAY_HPP
#define RAY_HPP

#include <custom/vec3.hpp>

class ray {
  /*N = C + mY
N: nokta
C: cikis yeri
m: mesafe/buyukluk
Y: yonu
   */
public:
  ray() {}
  ray(const nokta3 basNokta, const vek3 yon);
  nokta3 basi();
  vek3 yonu();
  nokta3 konum(float mesafe);

public:
  nokta3 bnokta;
  vek3 yon;
};
ray::ray(const nokta3 basNokta, const vek3 yon) {
  // 1. Constructor
  this->yon = yon;
  this->bnokta = basNokta;
}

nokta3 ray::basi() { return this->bnokta; }
nokta3 ray::yonu() { return this->yon; }
nokta3 ray::konum(float mesafe) { return this->yon * mesafe + this->bnokta; }

#endif
