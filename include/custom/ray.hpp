#ifndef RAY_HPP
#define RAY_HPP

#include <custom/vec3.hpp>

class Ray {
  /*N = C + mY
N: nokta
C: cikis yeri
m: mesafe/buyukluk
Y: yonu
   */
public:
  Ray() {}
  Ray(const nokta3 basNokta, const vek3 yon);
  nokta3 basi();
  vek3 yonu();
  nokta3 konum(double mesafe);

public:
  nokta3 bnokta;
  vek3 yon;
};
Ray::Ray(const nokta3 basNokta, const vek3 yon) {
  // 1. Constructor
  this->yon = yon;
  this->bnokta = basNokta;
}

nokta3 Ray::basi() { return this->bnokta; }
nokta3 Ray::yonu() { return this->yon; }
nokta3 Ray::konum(double mesafe) { return this->yon * mesafe + this->bnokta; }

#endif
