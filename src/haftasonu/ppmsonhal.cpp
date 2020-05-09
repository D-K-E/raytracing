// ppm ciktisi
//
#include <custom/vec3.hpp>
//
#include <custom/color.hpp>
//
#include <iostream>
//

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
      color pixel_rengi(r, g, b);
      write_color(std::cout, pixel_rengi);
    }
  }
  std::cout << std::endl;
  std::cerr << std::endl;
  std::cout << "Bitti" << std::endl;
  return 0; //
}
