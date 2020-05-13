// ppm ciktisi
//
#include <custom/vec3.hpp>
//
#include <custom/haftasonu/color.hpp>
//
#include <custom/haftasonu/ray.hpp>
//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
//

bool vurdu_kure(const point3 merkez, double yariCapi, Ray isin) {
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
  vec3 merkezeDogru = isin.origin - merkez;
  double a = dot(isin.direction, isin.direction);
  double b = 2.0 * dot(merkezeDogru, isin.direction);
  double c = dot(merkezeDogru, merkezeDogru) - yariCapi * yariCapi;
  double b2_minus_c = b * b - 4.0 * a * c;
  return (b2_minus_c >= 0.0);
}

color isin_rengi(Ray isin) {
  //
  point3 merkez(0.0, 0.0, -1.0);
  if (vurdu_kure(merkez, 0.5, isin)) {
    color r(1.0, 1.0, 0.0);
    return r;
  }
  vec3 yonu = glm::normalize(isin.direction);
  double mesafe = 0.5 * (yonu.y + 1.0);
  return (1.0 - mesafe) * color(1.0) + mesafe * color(0.5, 0.3, 1.0);
}

int main(void) {
  //
  const int resim_en = 256;
  const int resim_boy = 256;
  std::cout << "P3" << std::endl;
  std::cout << resim_en << ' ' << resim_boy << std::endl;
  std::cout << "255" << std::endl;
  vec3 baslangic(0.0, 0.0, 0.0);
  vec3 yatay(4.0, 0.0, 0.0);
  vec3 dikey(0.0, 2.25, 0.0);
  point3 asagi_sol =
      baslangic - yatay / 2.0 - dikey / 2.0 - vec3(0.0, 0.0, 1.0);

  // tersten yaziyoruz
  for (int j = resim_boy - 1; j >= 0; --j) {
    std::cerr << "\rKalan Tarama Çizgisi" << j << ' ' << std::flush;
    for (int i = 0; i < resim_en; ++i) {
      double u = double(i) / (resim_en - 1);
      double v = double(j) / (resim_boy - 1);
      Ray r(baslangic, asagi_sol + u * yatay + v * dikey);
      color isinin_rengi = isin_rengi(r);
      write_color(std::cout, isinin_rengi);
    }
  }
  std::cout << std::endl;
  std::cerr << std::endl;
  std::cout << "Bitti" << std::endl;
  return 0; //
}
