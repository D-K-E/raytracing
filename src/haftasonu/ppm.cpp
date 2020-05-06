// ppm ciktisi
#include <iostream>

int main(void) {
  //
  const int resim_en = 256;
  const int resim_boy = 256;
  std::cout << "P3\n" << resim_en << ' ' << resim_boy << "\n255\n";

  // tersten yaziyoruz
  for (int j = resim_boy - 1; j >= 0; --j) {
    for (int i = 0; i < resim_en; ++i) {
      double r = double(j) / (resim_en - 1);
      double g = double(j) / (resim_boy - 1);
      double b = 0.26;

      int ir = static_cast<int>(255.9 * r);
      int ig = static_cast<int>(255.9 * g);
      int ib = static_cast<int>(255.9 * b);

      std::cout << ir << ' ' << ig << ' ' << ib << '\n';
    }
  }
  std::cout << std::endl;
  return 0; //
}
