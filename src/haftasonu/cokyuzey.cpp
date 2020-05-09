//
#include <cmath>
#include <custom/color.hpp>
#include <custom/commons.hpp>
//
#include <custom/hittables.hpp>
//
#include <custom/sphere.hpp>
//
#include <iostream>
//
color ray_color(const Ray &r, const HittableList &scene) {
  // carpan isini renklendirir
  HitRecord record;
  if (scene.hit(r, 0, INF, record)) {
    return 0.5 * (record.normal + color(1.0));
  }
  vec3 direction = to_unit(r.direction);
  double temp = 0.5 * (direction.y + 1.0);
  return (1.0 - temp) * color(1.0) + temp * color(0.5, 0.8, 1.0);
}

int main(void) {
  // resmin yazma fonksiyonu
  double aspect_ratio = 16.0 / 9.0;
  const int imwidth = 386;
  const int imheight = static_cast<int>(imwidth / aspect_ratio);

  // ppm için gerekli olanlar
  std::cout << "P3" << std::endl;
  std::cout << imwidth << ' ' << imheight << std::endl;
  std::cout << "255" << std::endl;

  // ppm formati için gerekli olanlar son
  vec3 origin(0.0, 0.0, 0.0);
  vec3 hor(4.0, 0.0, 0.0);
  vec3 ver(0.0, 2.0, 0.0);
  vec3 lower_left_corner = vec3(-2.0, -1.0, -1.0);

  // konacak objelerin deklarasyonu
  HittableList scene;
  scene.add(make_shared<Sphere>(point3(0, 0, -1), 0.5));
  scene.add(make_shared<Sphere>(point3(0, -100.5, -1), 100));
  // deklarasyonu bitti

  // resim yazim
  for (int j = imheight - 1; j >= 0; --j) {
    std::cerr << "\rKalan Tarama Çizgisi:" << ' ' << j << ' ' << std::flush;
    for (int i = 0; i < imwidth; ++i) {
      double u = double(i) / (imwidth - 1);
      double v = double(j) / (imheight - 1);
      Ray r(origin, lower_left_corner + u * hor + v * ver);
      color rcolor = ray_color(r, scene);
      write_color(std::cout, rcolor);
    }
  }
}
