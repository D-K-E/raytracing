// ppm ciktisi
#include <iostream>

int main(void) {
  //
  const int resim_en = 256;
  const int resim_boy = 256;
  std::cout << "P3" << std::endl;
  std::cout << resim_en << ' ' << resim_boy << std::endl;
  std::cout << "255" << std::endl;

  // tersten yaziyoruz
  for (int j = resim_boy - 1; j >= 0; --j) {
    std::cerr << "\rKalan Tarama Çizgisi" << j << ' ' << std::flush;
    for (int i = 0; i < resim_en; ++i) {
      double r = double(j) / (resim_en - 1);
      double g = double(j) / (resim_boy - 1);
      double b = 0.26;

      int ir = static_cast<int>(255.9 * r);
      int ig = static_cast<int>(255.9 * g);
      int ib = static_cast<int>(255.9 * b);

      std::cout << ir << ' ' << ig << ' ' << ib << std::endl;
    }
  }
  std::cout << std::endl;
  std::cerr << "\nBitti" << std::endl;
  return 0; //
}
