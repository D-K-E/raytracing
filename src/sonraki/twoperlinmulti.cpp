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
#include <custom/sonraki/sphere.hpp>
//
//
#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#define THREAD_NB 4
//
using immat = std::vector<std::vector<color>>;

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

HittableList random_scene() {
  //
  HittableList scene;
  shared_ptr<NoiseTexture> pertext = make_shared<NoiseTexture>();
  scene.add(make_shared<Sphere>(point3(0, -1000, 0), 1000,
                                make_shared<Lambertian>(pertext)));
  scene.add(make_shared<Sphere>(point3(0, 2, 0), 2,
                                make_shared<Lambertian>(pertext)));
  return scene;
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
  immat imv;
  imv.resize(xrange);
  for (int i = 0; i < xrange; i++) {
    imv[i].resize(imheight, color(0));
  }

  for (int a = 0; a < xrange; a++) {
    for (int j = 0; j < imheight; j++) {
      int i = a + startx;
      //
      color rcolor(0.0, 0.0, 0.0);
      for (int k = 0; k < psample; k++) {
        double t = double(i + random_double()) / (imwidth - 1);
        double s = double(j + random_double()) / (imheight - 1);
        Ray r = camera.get_ray(t, s);
        rcolor += ray_color(r, scene, mdepth);
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
  const int imwidth = 640;
  const int imheight = static_cast<int>(imwidth / aspect_ratio);
  int psample = 100;
  int mdepth = 50;
  int arrlen = imwidth * imheight;
  int wslicelen = int(imwidth / THREAD_NB);
  immat imvec;
  imvec.resize(imwidth);
  for (int i = 0; i < imwidth; i++) {
    imvec[i].resize(imheight, color(0));
  }

  // ppm için gerekli olanlar
  std::cout << "P3" << std::endl;
  std::cout << imwidth << ' ' << imheight << std::endl;
  std::cout << "255" << std::endl;

  // konacak objelerin deklarasyonu
  HittableList scene = random_scene();

  // kamera
  vec3 vup(0, 1, 0);
  double dist_to_focus = 10.0;
  double aperature = 0.0;
  TimeRayCamera camera(point3(13, 2, 3), point3(0, 0, 0), vup, 45, aspect_ratio,
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
    params.psample = psample;
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
      write_color(std::cout, imvec[i][j], psample);
    }
  }
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

  std::cerr << "duration" << std::endl;

  std::cerr << duration.count() << std::endl;

  //
}
