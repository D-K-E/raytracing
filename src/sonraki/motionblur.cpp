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
#include <iostream>
#include <thread>
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

HittableList random_scene() {
  // random scene
  HittableList scene;
  scene.add(make_shared<Sphere>(point3(0, -1000, 0), 1000,
                                make_shared<Lambertian>(color(0.5, 0.5, 0.5))));

  int i = 1;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      double choose_mat = random_double();
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
      if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
        if (choose_mat < 0.8) {
          // diffuse
          vec3 albedo = random_vec() * random_vec();
          scene.add(make_shared<MovingSphere>(
              center,                                     // center1
              center + vec3(0, random_double(0, 0.5), 0), // center2
              0.0, 1.0, 0.2,                  // time1, time2, radius
              make_shared<Lambertian>(albedo) // albedo
              ));
        } else if (choose_mat < 0.95) {
          // metal
          vec3 albedo = random_vec(0.5, 1);
          double fuzz = random_double(0, 0.5);
          scene.add(make_shared<Sphere>(center, 0.2,
                                        make_shared<Metal>(albedo, fuzz)));
        } else {
          // glass
          scene.add(
              make_shared<Sphere>(center, 0.2, make_shared<Dielectric>(1.5)));
        }
      }
    }
  }

  scene.add(
      make_shared<Sphere>(point3(0, 1, 0), 1.0, make_shared<Dielectric>(1.5)));

  scene.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0,
                                make_shared<Lambertian>(color(.4, .2, .1))));

  scene.add(make_shared<Sphere>(point3(4, 1, 0), 1.0,
                                make_shared<Metal>(color(.7, .6, .5), 0.0)));
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
  //
  color rcolor(0.0, 0.0, 0.0);
  for (int k = 0; k < psample; k++) {
    double t = double(i + random_double()) / (imwidth - 1);
    double s = double(j + random_double()) / (imheight - 1);
    Ray r = camera.get_ray(t, s);
    rcolor += ray_color(r, scene, mdepth);
  }
  write_color(std::cout, rcolor, psample);
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
