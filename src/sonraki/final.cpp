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
#include <custom/sonraki/box.hpp>
#include <custom/sonraki/bvh.hpp>
#include <custom/sonraki/mediumc.hpp>
#include <custom/sonraki/sphere.hpp>
//
#include <chrono>
#include <filesystem>
#include <iostream>
//
#include <thread>
//
#include <future>
//
#define THREAD_NB 6
using immat = std::vector<std::vector<color>>;
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
  if (!scene.hit(r, 0.001, INF, record)) {
    return background;
  }
  // recursive case
  Ray r_out;
  color atten;
  color emittedColor =
      record.mat_ptr->emitted(record.u, record.v, record.point);
  if (!record.mat_ptr->scatter(r, record, atten, r_out)) {
    return emittedColor;
  }
  // bidirectional surface scattering distribution function
  // rendering equation = L^e + L^r
  return emittedColor + atten * ray_color(r_out, background, scene, depth - 1);
}

HittableList final_scene() {
  //
  fs::path texturePath = tpath / "someImg.jpg";
  HittableList objects;
  HittableList boxes1;
  auto ground =
      make_shared<Lambertian>(make_shared<SolidColor>(0.48, 0.83, 0.53));

  const int boxes_per_side = 20;
  for (int i = 0; i < boxes_per_side; i++) {
    for (int j = 0; j < boxes_per_side; j++) {
      auto w = 100.0;
      auto x0 = -1000.0 + i * w;
      auto z0 = -1000.0 + j * w;
      auto y0 = 0.0;
      auto x1 = x0 + w;
      auto y1 = random_double(1, 101);
      auto z1 = z0 + w;

      boxes1.add(
          make_shared<Box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
    }
  }
  objects.add(make_shared<BvhNode>(boxes1, 0, 1));

  auto light = make_shared<DiffuseLight>(make_shared<SolidColor>(7, 7, 7));
  objects.add(make_shared<XZRect>(123, 423, 147, 412, 554, light));

  auto center1 = point3(400, 400, 200);
  auto center2 = center1 + vec3(30, 0, 0);
  auto moving_sphere_material =
      make_shared<Lambertian>(make_shared<SolidColor>(0.7, 0.3, 0.1));
  objects.add(make_shared<MovingSphere>(center1, center2, 0, 1, 50,
                                        moving_sphere_material));

  objects.add(make_shared<Sphere>(point3(260, 150, 45), 50,
                                  make_shared<Dielectric>(1.5)));
  objects.add(make_shared<Sphere>(
      point3(0, 150, 145), 50, make_shared<Metal>(color(0.8, 0.8, 0.9), 10.0)));

  auto boundary = make_shared<Sphere>(point3(360, 150, 145), 70,
                                      make_shared<Dielectric>(1.5));
  objects.add(boundary);
  objects.add(make_shared<ConstantMedium>(
      boundary, 0.2, make_shared<SolidColor>(0.2, 0.4, 0.9)));
  boundary =
      make_shared<Sphere>(point3(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
  objects.add(make_shared<ConstantMedium>(boundary, .0001,
                                          make_shared<SolidColor>(1, 1, 1)));

  auto emat =
      make_shared<Lambertian>(make_shared<ImageTexture>(texturePath.c_str()));
  objects.add(make_shared<Sphere>(point3(400, 200, 400), 100, emat));
  auto pertext = make_shared<NoiseTexture>(0.1);
  objects.add(make_shared<Sphere>(point3(220, 280, 300), 80,
                                  make_shared<Lambertian>(pertext)));

  HittableList boxes2;
  auto white = make_shared<Lambertian>(make_shared<SolidColor>(.73, .73, .73));
  int ns = 1000;
  for (int j = 0; j < ns; j++) {
    boxes2.add(make_shared<Sphere>(random_vec(0, 165), 10, white));
  }

  objects.add(make_shared<Translate>(
      make_shared<RotateY>(make_shared<BvhNode>(boxes2, 0.0, 1.0), 15),
      vec3(-100, 270, 395)));

  return objects;
}

struct InnerParams {
  TimeRayCamera camera;
  int imwidth;
  int imheight;
  int psample;
  int mdepth;
  int startx;
  int endx;
  HittableList scene;
};

struct InnerRet {
  immat img;
  int startx;
  int endx;
};
InnerRet innerLoop(InnerParams params) {
  // inner loop for write_color
  TimeRayCamera camera = params.camera;
  int imwidth = params.imwidth;
  int imheight = params.imheight;
  int psample = params.psample;
  int mdepth = params.mdepth;
  int startx = params.startx;
  int endx = params.endx;
  int xrange = endx - startx;
  HittableList scene = params.scene;
  immat imv(xrange, std::vector<color>(imheight, color(0)));
  color background(0);

  for (int a = 0; a < xrange; a++) {
    for (int j = 0; j < imheight; j++) {
      int i = a + startx;
      //
      color rcolor(0.0, 0.0, 0.0);
      for (int k = 0; k < psample; k++) {
        double t = double(i + random_double()) / (imwidth - 1);
        double s = double(j + random_double()) / (imheight - 1);
        Ray r = camera.get_ray(t, s);
        rcolor += ray_color(r, background, scene, mdepth);
      }
      imv[a][j] = rcolor;
    }
  }
  InnerRet ret;
  ret.img = imv;
  ret.startx = startx;
  ret.endx = endx;
  return ret;
}

int main(void) {
  auto start = std::chrono::high_resolution_clock::now();
  // resmin yazma fonksiyonu
  double aspect_ratio = 16.0 / 9.0;
  const int imwidth = 800;
  const int imheight = static_cast<int>(imwidth / aspect_ratio);
  int pixel_sample = 1000;
  int mdepth = 100;
  int wslicelen = int(imwidth / THREAD_NB);
  immat imvec(imwidth, std::vector<color>(imheight, color(0)));

  // ppm için gerekli olanlar
  std::cout << "P3" << std::endl;
  std::cout << imwidth << ' ' << imheight << std::endl;
  std::cout << "255" << std::endl;

  // konacak objelerin deklarasyonu
  HittableList scene = final_scene();

  // kamera
  vec3 vup(0, 1, 0);
  double dist_to_focus = 10.0;
  double aperature = 0.0;
  point3 corigin = point3(278, 278, -800);
  point3 target = point3(278, 278, 0);
  double vertical_fov = 40.0;
  TimeRayCamera camera(corigin, target, vup, vertical_fov, aspect_ratio,
                       aperature, dist_to_focus, 0.0, 1.0);

  std::vector<std::future<InnerRet>> futures(THREAD_NB);

  // resim yazim
  for (int t = 0; t < THREAD_NB; t++) {
    int startx = wslicelen * t;
    int endx = fmin(startx + wslicelen, imwidth);
    InnerParams params;
    params.startx = startx;
    params.endx = endx;
    params.camera = camera;
    params.imwidth = imwidth;
    params.imheight = imheight;
    params.psample = pixel_sample;
    params.mdepth = mdepth;
    params.scene = scene;
    futures[t] = std::async(&innerLoop, params);
  }
  for (auto &f : futures) {
    InnerRet ret = f.get();
    immat img = ret.img;
    int startx = ret.startx;
    int endx = ret.endx;
    int xrange = endx - startx;
    for (int i = 0; i < xrange; i++) {
      for (int j = 0; j < imheight; j++) {
        imvec[i + startx][j] = img[i][j];
      }
    }
  }
  for (int j = imheight - 1; j >= 0; j -= 1) {
    std::cerr << "\rKalan Tarama Çizgisi:" << ' ' << j << ' ' << std::flush;
    for (int i = 0; i < imwidth; i += 1) {
      write_color(std::cout, imvec[i][j], pixel_sample);
    }
  }
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  std::cerr << "duration" << std::endl;
  std::cerr << duration.count() << std::endl;
  //
}
