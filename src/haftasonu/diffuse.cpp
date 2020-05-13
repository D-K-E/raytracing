//
#include <cmath>
#include <custom/haftasonu/color.hpp>
#include <custom/haftasonu/commons.hpp>
//
#include <custom/haftasonu/hittables.hpp>
//
#include <custom/haftasonu/camera.hpp>
#include <custom/haftasonu/sphere.hpp>
//
#include <iostream>
//
color ray_color(const Ray &r, const HittableList &scene, int depth) {
  // carpan isini renklendirir
  HitRecord record;
  if (depth <= 0) {
    // final case
    return color(0);
  }
  if (scene.hit(r, 0.001, INF, record)) {
    // recursive case
    point3 target = record.point + random_in_hemisphere(record.normal);
    Ray r2 = Ray(record.point, target - record.point);
    return 0.5 * ray_color(r2, scene, depth - 1); // advancing condition
  }
  vec3 direction = to_unit(r.direction);
  double temp = 0.5 * (direction.y + 1.0);
  return (1.0 - temp) * color(1.0) + temp * color(0.5, 0.7, 1.0);
}

int main(void) {
  // resmin yazma fonksiyonu
  double aspect_ratio = 16.0 / 9.0;
  const int imwidth = 640;
  const int imheight = static_cast<int>(imwidth / aspect_ratio);
  int psample = 100;
  int mdepth = 50;

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
  scene.add(make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0));
  scene.add(make_shared<Sphere>(point3(0.0, 0.0, -1.0), 0.5));
  // deklarasyonu bitti

  // kamera
  RayCamera camera;

  // resim yazim
  for (int j = imheight - 1; j >= 0; --j) {
    std::cerr << "\rKalan Tarama Çizgisi:" << ' ' << j << ' ' << std::flush;
    for (int i = 0; i < imwidth; ++i) {
      color rcolor(0.0, 0.0, 0.0);
      for (int s = 0; s < psample; s++) {
        double u = double(i + random_double()) / (imwidth - 1);
        double v = double(j + random_double()) / (imheight - 1);
        Ray r = camera.get_ray(u, v);
        rcolor += ray_color(r, scene, mdepth);
      }
      write_color(std::cout, rcolor, psample);
    }
  }
}
