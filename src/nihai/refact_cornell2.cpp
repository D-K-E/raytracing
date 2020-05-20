//
#include <cmath>
//
#include <custom/nihai/commons.hpp>
//
#include <custom/nihai/color.hpp>
//
#include <custom/nihai/hittables.hpp>
//
#include <custom/nihai/camera.hpp>
//
#include <custom/nihai/aarect.hpp>
#include <custom/nihai/box.hpp>
#include <custom/nihai/bvh.hpp>
#include <custom/nihai/mediumc.hpp>
#include <custom/nihai/sphere.hpp>
//
#include <chrono>
#include <filesystem>
#include <iostream>
//
#include <thread>
//
#include <future>
//
#define THREAD_NB 5
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
  if (scene.hit(r, 0.001, INF, record) == false) {
    return background;
  }
  // recursive case
  Ray r_out;
  color atten;
  color emittedColor =
      record.mat_ptr->emitted(record.u, record.v, record.point);
  color albed;
  double pdf;
  if (record.mat_ptr->scatter(r, record, albed, r_out, pdf) == false) {
    return emittedColor;
  }
  color Le = emittedColor;
  color Lr = albed;
  // std::cerr << "Lr0: " << Lr << std::endl;
  double pdfscatter = record.mat_ptr->pdf_scattering(r, record, r_out);
  Lr *= pdfscatter;
  // std::cerr << "Lr1: " << Lr << std::endl;
  color rcolor = ray_color(r_out, background, scene, depth - 1);

  Lr *= rcolor; // std::cerr << "Lr2: " << Lr << std::endl;
  Lr /= pdf;
  // std::cerr << "Lr3: " << Lr << std::endl;
  // bidirectional surface scattering distribution function
  // rendering equation = L^e + L^r
  // where L^r is = brdf or bsdf or bssrdf
  return Le + Lr;
}

HittableList cornell_box(TimeRayCamera &cam, double aspect_ratio) {
  //
  HittableList scene;

  // ---------- materials -----------------
  auto red = make_shared<Lambertian>(make_shared<SolidColor>(0.65, 0.05, 0.05));
  auto white =
      make_shared<Lambertian>(make_shared<SolidColor>(0.88, 0.88, 0.88));
  auto green =
      make_shared<Lambertian>(make_shared<SolidColor>(0.18, 0.78, 0.18));
  auto light =
      make_shared<DiffuseLight>(make_shared<SolidColor>(15.0, 15.0, 15.0));

  // --------- objects -------------------
  scene.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
  scene.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
  scene.add(make_shared<XZRect>(213, 343, 227, 332, 554, light));
  scene.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
  scene.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
  scene.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));
  shared_ptr<Hittable> box1 =
      make_shared<Box>(point3(0), point3(165, 330, 165), white);
  box1 = make_shared<RotateY>(box1, 15);
  box1 = make_shared<Translate>(box1, vec3(265, 0, 295));
  scene.add(box1);
  shared_ptr<Hittable> box2 = make_shared<Box>(point3(0), point3(165), white);
  box2 = make_shared<RotateY>(box2, -18);
  box2 = make_shared<Translate>(box2, vec3(130, 0, 65));
  scene.add(box2);

  point3 lookfrom(278, 278, -800);
  point3 lookat(278, 278, 0);
  vec3 vup(0, 1, 0);
  auto dist_to_focus = 10.0;
  auto aperture = 0.0;
  auto vfov = 40.0;
  auto t0 = 0.0;
  auto t1 = 1.0;

  cam = TimeRayCamera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture,
                      dist_to_focus, t0, t1);

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

InnerParams makeInner(double startx, double endx, const TimeRayCamera &camera,
                      double imwidth, double imheight, double pixel_sample,
                      double mdepth, const HittableList &scene) {
  InnerParams params;
  params.startx = startx;
  params.endx = endx;
  params.camera = camera;
  params.imwidth = imwidth;
  params.imheight = imheight;
  params.psample = pixel_sample;
  params.mdepth = mdepth;
  params.scene = scene;
  return params;
}

struct InnerRet {
  immat img;
  int startx;
  int endx;
};
void joinRet2Imv(InnerRet ret, immat &imvec, double imheight) {
  // join returned promise to image
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
  const int imwidth = 640;
  const int imheight = static_cast<int>(imwidth / aspect_ratio);
  int pixel_sample = 200;
  int mdepth = 50;
  int wslicelen = int(imwidth / THREAD_NB);
  immat imvec(imwidth, std::vector<color>(imheight, color(0)));

  // ppm için gerekli olanlar
  std::cout << "P3" << std::endl;
  std::cout << imwidth << ' ' << imheight << std::endl;
  std::cout << "255" << std::endl;

  TimeRayCamera camera;
  // konacak objelerin deklarasyonu
  HittableList scene = cornell_box(camera, aspect_ratio);

  // kamera

  std::vector<std::future<InnerRet>> futures(THREAD_NB);

  // resim yazim
  for (int t = 0; t < THREAD_NB; t++) {
    int startx = wslicelen * t;
    int endx = fmin(startx + wslicelen, imwidth);
    InnerParams params = makeInner(startx, endx, camera, imwidth, imheight,
                                   pixel_sample, mdepth, scene);
    futures[t] = std::async(&innerLoop, params);
  }
  for (auto &f : futures) {
    InnerRet ret = f.get();
    joinRet2Imv(ret, imvec, imheight);
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
