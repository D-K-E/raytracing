#ifndef TEXTURE_HPP
#define TEXTURE_HPP
//
#define STB_IMAGE_IMPLEMENTATION
#include <custom/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <custom/stb_image_write.h>
//
#include <custom/nihai/commons.hpp>
#include <custom/nihai/perlin2.hpp>
//
//
// ------------ texture utils ----------------------

/*
void get_sphere_uv(const vec3 &outNormal, double &u, double &v) {
  // http://www.cse.msu.edu/~cse872/tutorial4.html
  //    The interesting problem for the sphere is to determine the texture
  //    coordinates
  //    for each vertex.  One way to look at this problem is to use the
surface
  //    normal
  //    as way to tell where we are on the face of the sphere.  If we
consider
  //    this a
  //    vector from the inside of the sphere, we can easily compute the
latitude
  //    and
  //    longitude on the surface of the globe.  These values correspond to
point
  //    on the
  //    map.

  //    Right after the call to glBegin(GL_TRIANGLES), add the following
code:

  //      // What's the texture coordinate for this normal?
  //      tx1 = atan2(a[0], a[2]) / (2. * GR_PI) + 0.5;
  //      ty1 = asin(a[1]) / GR_PI + .5;

  //      glTexCoord2f(tx1, ty1);

  //    So, what does this do?  a[0] and a[2] are the X and Z values of the
  //    normal.  If
  //    you look straight down onto the globe from the top, the vector made
up
  //    of the X
  //    and Z values will tell you the longitude on the globe!  I use atan2
to
  //    convert
  //    that to an angle in radians.  This angle is between -PI and PI.  I
  //    divide by
  //    2PI, so it's now between -.5 and .5.  Adding 0.5 makes it range from
0
  //    to 1.
  //    This is the X value in the texture map.
  //    double phi = atan2(outNormal.x, outNormal.z);
  //    double theta = asin(outNormal.y);
  //    u = 0.5 + (phi / 2 * PI);
  //    v = 0.5 + (theta / PI);
  auto phi = atan2(outNormal.z, outNormal.x);
  auto theta = asin(outNormal.y);
  u = 1 - (phi + PI) / (2 * PI);
  v = (theta + PI / 2) / PI;
}
*/

// -------------------------------------------------
class texture {
public:
  virtual color value(double u, double v, const vec3 &p) const = 0;
};

class solid_color : public texture {
public:
  solid_color() {}
  solid_color(color c) : color_value(c) {}

  solid_color(double red, double green, double blue)
      : solid_color(color(red, green, blue)) {}

  virtual color value(double u, double v, const vec3 &p) const {
    return color_value;
  }

private:
  color color_value;
};

class checker_texture : public texture {
public:
  checker_texture() {}
  checker_texture(shared_ptr<texture> t0, shared_ptr<texture> t1)
      : even(t0), odd(t1) {}

  virtual color value(double u, double v, const vec3 &p) const {
    auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
    if (sines < 0)
      return odd->value(u, v, p);
    else
      return even->value(u, v, p);
  }

public:
  shared_ptr<texture> odd;
  shared_ptr<texture> even;
};

class noise_texture : public texture {
public:
  noise_texture() {}
  noise_texture(double sc) : scale(sc) {}

  virtual color value(double u, double v, const vec3 &p) const {
    // return color(1,1,1)*0.5*(1 + noise.turb(scale * p));
    // return color(1,1,1)*noise.turb(scale * p);
    return color(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.turb(p)));
  }

public:
  perlin noise;
  double scale;
};

class image_texture : public texture {
public:
  const static int bytes_per_pixel = 3;

  image_texture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

  image_texture(const char *filename) {
    auto components_per_pixel = bytes_per_pixel;

    data = stbi_load(filename, &width, &height, &components_per_pixel,
                     components_per_pixel);

    if (!data) {
      std::cerr << "ERROR: Could not load texture image file '" << filename
                << "'.\n";
      width = height = 0;
    }

    bytes_per_scanline = bytes_per_pixel * width;
  }

  ~image_texture() { delete data; }

  virtual color value(double u, double v, const vec3 &p) const {
    // If we have no texture data, then return solid cyan as a debugging aid.
    if (data == nullptr)
      return color(0, 1, 1);

    // Clamp input texture coordinates to [0,1] x [1,0]
    u = clamp(u, 0.0, 1.0);
    v = 1.0 - clamp(v, 0.0, 1.0); // Flip V to image coordinates

    auto i = static_cast<int>(u * width);
    auto j = static_cast<int>(v * height);

    // Clamp integer mapping, since actual coordinates should be less than 1.0
    if (i >= width)
      i = width - 1;
    if (j >= height)
      j = height - 1;

    const auto color_scale = 1.0 / 255.0;
    auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

    return color(color_scale * pixel[0], color_scale * pixel[1],
                 color_scale * pixel[2]);
  }

private:
  unsigned char *data;
  int width, height;
  int bytes_per_scanline;
};

#endif
