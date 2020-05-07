// ppm ciktisi
//
#include <custom/vec3.hpp>
//
#include <custom/renk.hpp>
//
#include <custom/ray.hpp>
//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
//

bool vurdu_kure(const nokta3 merkez, double yariCapi, ray isin) {
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
  vek3 merkezeDogru = isin.basi() - merkez;
  double a = glm::dot(isin.yonu(), isin.yonu());
  double b = 2.0 * glm::dot(merkezeDogru, isin.yonu());
  double c = glm::dot(merkezeDogru, merkezeDogru) - yariCapi * yariCapi;
  double b2_minus_c = b * b - 4.0 * a * c;
  return (b2_minus_c >= 0.0);
}

renk isin_rengi(ray isin) {
  //
  nokta3 merkez(0.0, 0.0, -1.0);
  if (vurdu_kure(merkez, 0.5, isin)) {
    renk r(1.0, 1.0, 0.0);
    return r;
  }
  vek3 yonu = glm::normalize(isin.yon);
  double mesafe = 0.5 * (yonu.y + 1.0);
  return (1.0 - mesafe) * renk(1.0) + mesafe * renk(0.5, 0.3, 1.0);
}

int main(void) {
  //
  const int resim_en = 256;
  const int resim_boy = 256;
  std::cout << "P3" << std::endl;
  std::cout << resim_en << ' ' << resim_boy << std::endl;
  std::cout << "255" << std::endl;
  vek3 baslangic(0.0, 0.0, 0.0);
  vek3 yatay(4.0, 0.0, 0.0);
  vek3 dikey(0.0, 2.25, 0.0);
  nokta3 asagi_sol =
      baslangic - yatay / 2.0 - dikey / 2.0 - vek3(0.0, 0.0, 1.0);

  // tersten yaziyoruz
  for (int j = resim_boy - 1; j >= 0; --j) {
    std::cerr << "\rKalan Tarama Çizgisi" << j << ' ' << std::flush;
    for (int i = 0; i < resim_en; ++i) {
      double u = double(i) / (resim_en - 1);
      double v = double(j) / (resim_boy - 1);
      ray r(baslangic, asagi_sol + u * yatay + v * dikey);
      renk isinin_rengi = isin_rengi(r);
      renk_yaz(std::cout, isinin_rengi);
    }
  }
  std::cout << std::endl;
  std::cerr << std::endl;
  std::cout << "Bitti" << std::endl;
  return 0; //
}
