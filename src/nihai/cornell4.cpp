#include "utils.hpp"

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
      record.mat_ptr->emitted(r, record, record.u, record.v, record.point);

  // ------------- Ray hits surface -----------------------------

  color albed;
  double pdf_val;
  if (record.mat_ptr->scatter(r, record, albed, r_out, pdf_val) == false) {
    return emittedColor;
  }

  // ------------ Ray hits surface ends -------------------------

  // ------------ sampling light ----------------------

  shared_ptr<Hittable> lightShape =
      make_shared<XZRect>(213, 343, 227, 332, 554, make_shared<Material>());

  // HittablePdf hp(lightShape, record.point);
  auto hp = make_shared<HittablePdf>(lightShape, record.point);

  // other pdfs
  // CosinePdf cpdf(record.normal);
  auto cpdf = make_shared<CosinePdf>(record.normal);
  // MixturePdf mpdf(&hp, &cpdf);
  MixturePdf mpdf(hp, cpdf);

  //
  r_out = Ray(record.point, hp->generate(), r.time());
  pdf_val = hp->value(r_out.dir());

  // --------------------------------------------------
  color Le = emittedColor; //
  color Lr = albed;

  double pdfscatter = record.mat_ptr->pdf_scattering(r, record, r_out);
  Lr *= pdfscatter;

  color rcolor = ray_color(r_out, background, scene, depth - 1);

  Lr *= rcolor; // std::cerr << "Lr2: " << Lr << std::endl;
  Lr /= pdf_val;

  // bidirectional surface scattering distribution function
  // rendering equation = L^e + L^r
  // where L^r is = brdf or bsdf or bssrdf
  // rendering equation

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
      make_shared<DiffuseLight>(make_shared<SolidColor>(25.0, 25.0, 25.0));

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

int main(void) {
  auto start = std::chrono::high_resolution_clock::now();
  // resmin yazma fonksiyonu
  double aspect_ratio = 16.0 / 9.0;
  const int imwidth = 600;
  const int imheight = static_cast<int>(imwidth / aspect_ratio);
  int pixel_sample = 100;
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
