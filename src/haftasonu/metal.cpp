//
#include <cmath>
#include <custom/color.hpp>
#include <custom/commons.hpp>
//
#include <custom/hittables.hpp>
//
#include <custom/camera.hpp>
#include <custom/sphere.hpp>
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
    Ray r_out;
    color atten;
    if (record.mat_ptr->scatter(r, record, atten, r_out)) {
      return atten * ray_color(r_out, scene, depth - 1);
    }
    return color(1.0);
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

  // konacak objelerin deklarasyonu
  HittableList scene;
  scene.add(make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0,
                                make_shared<Lambertian>(color(0.8, 0.8, 0.0))));

  scene.add(make_shared<Sphere>(point3(1, 0, -1), 0.5,
                                make_shared<Metal>(color(0.8, 0.6, 0.2), 0.3)));
  // içi bos kure eksi yari çap normalleri içeri dogru yoneltiyor
  // bu balonumsu bir goruntu yakalamak için ideal

  scene.add(make_shared<Sphere>(point3(-1, 0, -1), 0.5,
                                make_shared<Dielectric>(1.5)));
  scene.add(make_shared<Sphere>(point3(-1, 0, -1), -0.45,
                                make_shared<Dielectric>(1.5)));
  // deklarasyonu bitti

  scene.add(make_shared<Sphere>(point3(0.0, 0.0, -1.0), 0.5,
                                make_shared<Lambertian>(color(0.1, 0.2, 0.5))));

  // kamera
  vec3 vup(0, 1, 0);
  RayCamera camera(point3(-2, 2, 1), point3(0, 0, -1), vup, 45, aspect_ratio);

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
