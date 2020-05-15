#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <custom/sonraki/commons.hpp>
#include <custom/sonraki/perlin.hpp>
//
#define STB_IMAGE_IMPLEMENTATION
#include <custom/stb_image.h>
//

class Texture {
public:
  virtual color value(double u, double v, const point3 &p) const = 0;
};

class SolidColor : public Texture {
private:
  color color_value;

public:
  SolidColor() {}
  SolidColor(color c) : color_value(c) {}
  SolidColor(double red, double green, double blue)
      : color_value(color(red, green, blue)) {}
  virtual color value(double u, double v, const point3 &p) const {
    return color_value;
  }
};
class CheckerTexture : public Texture {
private:
  shared_ptr<Texture> odd;
  shared_ptr<Texture> even;

public:
  CheckerTexture() {}
  CheckerTexture(shared_ptr<Texture> t1, shared_ptr<Texture> t2)
      : odd(t1), even(t2) {}
  virtual color value(double u, double v, const point3 &p) const {
    double sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
    if (sines < 0) {
      return odd->value(u, v, p);
    } else {
      return even->value(u, v, p);
    }
  }
};
class NoiseTexture : public Texture {
public:
  NoiseTexture() {}
  virtual color value(double u, double v, const point3 &p) const {
    auto tmp = 1.0 + sin(scale * p.z + 10 * noise.turb(p));
    return color(1) * 0.5 * tmp;
  }

public:
  Perlin noise;
  double scale;
};
class ImageTexture : public Texture {
  /*
     Texture mapping and sampling
So instead, one of the the most universal unofficial standards in graphics is to
use texture coordinates instead of image pixel coordinates. These are just some
form of fractional position in the image. For example, for pixel (i,j) in an Nx
by Ny image, the image texture position is:

u=i / Nx−1
v=j / Ny−1

This is just a fractional position. For a hittable, we need to also return a u
and v in the hit record.

For spheres:
u = phi / 2pi
v = theta / pi
phi = atan2(y,x)
theta = asin(z)
   */
public:
  const static int bytes_per_pixel_jpg = 3;
  const static int bytes_per_pixel_png = 4;

private:
  unsigned char *imdata;
  int width, height;
  int bytes_per_scanline;
  int bytes_per_pixel;

public:
  ImageTexture()
      : imdata(nullptr), width(0), height(0), bytes_per_scanline(0) {}
  ImageTexture(const char *filename) {
    // simple support for png files as well
    int components =
        strstr(filename, "jpg") ? bytes_per_pixel_jpg : bytes_per_pixel_png;
    bytes_per_pixel = components;
    // std::cerr << "components " << components << std::endl;
    auto data = stbi_load(filename, &width, &height, &components, components);
    if (data == nullptr) {
      std::cerr << "Could not load image from " << filename << std::endl;
      width = height = 0;
    }
    imdata = data;
    bytes_per_scanline = components * width;
  }
  ~ImageTexture() { delete imdata; }
  virtual color value(double u, double v, const vec3 &p) const {
    //
    if (imdata == nullptr) {
      // image not loaded is returned as cyan for debugging
      std::cerr << "imdata ------" << std::endl;
      return color(0.0, 0, 1);
    }
    // clamp input texture coordinates [0,1] x [1,0]
    u = clamp(u, 0.0, 1.0);
    v = 1.0 - clamp(v, 0.0, 1.0);

    int i = static_cast<int>(u * width);
    int j = static_cast<int>(v * height);
    if (i >= width) {
      i = width - 1;
    }
    if (j >= height) {
      j = height - 1;
    }
    const double color_scale = 1.0 / 255.0;
    unsigned char *pixel =
        imdata + j * bytes_per_scanline + i * bytes_per_pixel;
    return color(color_scale * pixel[0], color_scale * pixel[1],
                 color_scale * pixel[2]);
  }
};

#endif
