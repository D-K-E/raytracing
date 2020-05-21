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
//
#include <custom/nihai/pdf.hpp>
//
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
#define THREAD_NB 6
using immat = std::vector<std::vector<color>>;
//
namespace fs = std::filesystem;
fs::path cdir = fs::current_path();
fs::path tpath = cdir / "texture";

color ray_color(const Ray &r, const Hittable &scene, int depth);

color ray_color(const Ray &r, const color &background,
                const HittableList &scene, int depth);

color ray_color(const Ray &r, const color &background,
                const HittableList &scene, shared_ptr<Hittable> lights,
                int depth);

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
