//
#include <cmath>
#include <custom/sonraki/color.hpp>
//
#include <custom/sonraki/commons.hpp>
//
#include <custom/sonraki/hittables.hpp>
//
#include <custom/sonraki/camera.hpp>
//
#include <custom/sonraki/aarect.hpp>
#include <custom/sonraki/material.hpp>
#include <custom/sonraki/sphere.hpp>
//
#include <filesystem>
#include <iostream>
#include <thread>
//
namespace fs = std::filesystem;
fs::path cdir = fs::current_path();
fs::path tpath = cdir / "texture";

color ray_color(const Ray &r, const color &background,
                const HittableList &scene, int depth) {
  // carpan isini renklendirir
  HitRecord record;
  if (depth <= 0) {
    // final case
    return color(0);
  }
  if (scene.hit(r, 0.001, INF, record) == false) {
    return background;
  } else {
    // recursive case
    Ray r_out;
    color atten;
    color emittedColor =
        record.mat_ptr->emitted(record.u, record.v, record.point);
    if (record.mat_ptr->scatter(r, record, atten, r_out) == false) {
      // std::cerr << "no scatter record: " << emittedColor << std::endl;
      return emittedColor;
    } else {
      // return emittedColor;

      color rcolor = ray_color(r_out, background, scene, depth - 1);
      rcolor *= atten;
      rcolor += emittedColor;

      return rcolor;
    }
  }
}

HittableList random_scene() {
  //
  HittableList scene;

  // -------------- preparing textures --------------------
  auto pertext = make_shared<NoiseTexture>(4);
  auto scolor = make_shared<SolidColor>(4.0, 4.0, 4.0);
  // -------------- end textures --------------------

  // -------------- preparing objects --------------------
  scene.add(make_shared<Sphere>(point3(0, -1000, 0), 1000,
                                make_shared<Lambertian>(pertext)));
  scene.add(make_shared<Sphere>(point3(0, 2, 0), 2,
                                make_shared<Lambertian>(pertext)));

  auto difflight = make_shared<DiffuseLight>(scolor);
  scene.add(make_shared<Sphere>(point3(0, 7, 0), 2, difflight));
  scene.add(make_shared<XYRect>(3, 5, 1, 3, -2, difflight));
  // -------------- end objects --------------------
  return scene;
}

struct InnerParams {
  TimeRayCamera camera;
  int imwidth;
  int imheight;
  int psample;
  int mdepth;
  int imi;
  int imj;
  HittableList scene;
};

void innerLoop(InnerParams params) {
  // inner loop for write_color
  TimeRayCamera camera = params.camera;
  int imwidth = params.imwidth;
  int imheight = params.imheight;
  int psample = params.psample;
  int mdepth = params.mdepth;
  int i = params.imi;
  int j = params.imj;
  HittableList scene = params.scene;
  color background(0);
  //
  color rcolor(0.0, 0.0, 0.0);
  for (int k = 0; k < psample; k++) {
    double t = double(i + random_double()) / (imwidth - 1);
    double s = double(j + random_double()) / (imheight - 1);
    Ray r = camera.get_ray(t, s);
    rcolor += ray_color(r, background, scene, mdepth);
  }
  write_color(std::cout, rcolor, psample);
}

int main(void) {
  // resmin yazma fonksiyonu
  double aspect_ratio = 16.0 / 9.0;
  const int imwidth = 640;
  const int imheight = static_cast<int>(imwidth / aspect_ratio);
  int psample = 200;
  int mdepth = 75;

  // ppm için gerekli olanlar
  std::cout << "P3" << std::endl;
  std::cout << imwidth << ' ' << imheight << std::endl;
  std::cout << "255" << std::endl;

  // konacak objelerin deklarasyonu
  HittableList scene = random_scene();

  // kamera
  vec3 vup(0, 1, 0);
  double dist_to_focus = 10.0;
  double aperature = 0.1;
  TimeRayCamera camera(point3(13, 2, 3), point3(0, 0, 0), vup, 45, aspect_ratio,
                       aperature, dist_to_focus, 0.0, 1.0);

  // resim yazim
  for (int j = imheight - 1; j >= 0; j -= 1) {
    std::cerr << "\rKalan Tarama Çizgisi:" << ' ' << j << ' ' << std::flush;
    for (int i = 0; i < imwidth; i += 1) {
      InnerParams params1 = {camera, imwidth, imheight, psample,
                             mdepth, i,       j,        scene};
      innerLoop(params1);
      //
    }
  }
}
