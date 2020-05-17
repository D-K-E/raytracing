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
#define THREAD_NB 4
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

HittableList cornell_box() {
  //
  HittableList scene;

  // ---------- materials -----------------
  auto red = make_shared<Lambertian>(make_shared<SolidColor>(0.65, 0.05, 0.05));
  auto white =
      make_shared<Lambertian>(make_shared<SolidColor>(0.88, 0.88, 0.88));
  auto green =
      make_shared<Lambertian>(make_shared<SolidColor>(0.18, 0.78, 0.18));
  auto light =
      make_shared<DiffuseLight>(make_shared<SolidColor>(25.0, 25.0, 25.0));

  // --------- objects -------------------
  scene.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
  scene.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
  scene.add(make_shared<XZRect>(213, 343, 227, 332, 554, light));
  scene.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
  scene.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
  scene.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));

  shared_ptr<Hittable> box1 =
      make_shared<Box>(point3(0, 0, 0), point3(165, 330, 165), white);
  box1 = make_shared<RotateY>(box1, 15);
  box1 = make_shared<Translate>(box1, vec3(265, 0, 295));

  shared_ptr<Hittable> box2 =
      make_shared<Box>(point3(0, 0, 0), point3(165), white);
  box2 = make_shared<RotateY>(box2, -18);
  box2 = make_shared<Translate>(box2, vec3(130, 0, 65));

  scene.add(box1);
  scene.add(box2);

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
  int psample = 200;
  int mdepth = 100;
  int arrlen = imwidth * imheight;
  int wslicelen = int(imwidth / THREAD_NB);
  immat imvec(imwidth, std::vector<color>(imheight, color(0)));

  // ppm için gerekli olanlar
  std::cout << "P3" << std::endl;
  std::cout << imwidth << ' ' << imheight << std::endl;
  std::cout << "255" << std::endl;

  // konacak objelerin deklarasyonu
  HittableList scene = cornell_box();

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
