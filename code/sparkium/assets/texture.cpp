#include "sparkium/assets/texture.h"

#include "cassert"
#include "glm/gtc/matrix_transform.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace sparkium {
Texture::Texture(uint32_t width, uint32_t height, const glm::vec4 &color)
    : Texture(width, height, std::vector<glm::vec4>(width * height, color)) {
}

Texture::Texture(uint32_t width,
                 uint32_t height,
                 const std::vector<glm::vec4> &pixels) {
  assert(width * height == pixels.size());
  width_ = width;
  height_ = height;
  pixels_ = pixels;
}

int Texture::LoadFromFile(const std::string &file_path,
                          LDRColorSpace ldr_color_space) {
  float restore_gamma = stbi__l2h_gamma;
  switch (ldr_color_space) {
    case LDRColorSpace::SRGB:
      stbi_ldr_to_hdr_gamma(2.2f);
      break;
    case LDRColorSpace::UNORM:
      stbi_ldr_to_hdr_gamma(1.0f);
      break;
  }

  float *pixels =
      stbi_loadf(file_path.c_str(), reinterpret_cast<int *>(&width_),
                 reinterpret_cast<int *>(&height_), nullptr, 4);
  stbi_ldr_to_hdr_gamma(restore_gamma);
  if (!pixels) {
    return -1;
  }
  pixels_.resize(width_ * height_);
  for (int i = 0; i < width_ * height_; i++) {
    pixels_[i] = glm::vec4{pixels[i * 4], pixels[i * 4 + 1], pixels[i * 4 + 2],
                           pixels[i * 4 + 3]};
  }
  stbi_image_free(pixels);
  return 0;
}

int Texture::SaveToFile(const std::string &file_path,
                        LDRColorSpace ldr_color_space) const {
  // If ends with .hdr
  if (file_path.size() >= 4 &&
      file_path.substr(file_path.size() - 4) == ".hdr") {
    std::vector<float> float_pixels(pixels_.size() * 4);
    for (int i = 0; i < pixels_.size(); i++) {
      float_pixels[i * 4] = pixels_[i].r;
      float_pixels[i * 4 + 1] = pixels_[i].g;
      float_pixels[i * 4 + 2] = pixels_[i].b;
      float_pixels[i * 4 + 3] = pixels_[i].a;
    }
    stbi_write_hdr(file_path.c_str(), width_, height_, 4, float_pixels.data());
  } else {
    std::vector<uint8_t> byte_pixels = ConvertTexture(*this, ldr_color_space);
    if (file_path.size() >= 4 &&
        file_path.substr(file_path.size() - 4) == ".png") {
      stbi_write_png(file_path.c_str(), width_, height_, 4, byte_pixels.data(),
                     width_ * 4);
    } else if (file_path.size() >= 4 &&
               file_path.substr(file_path.size() - 4) == ".bmp") {
      stbi_write_bmp(file_path.c_str(), width_, height_, 4, byte_pixels.data());
    } else if (file_path.size() >= 4 &&
               file_path.substr(file_path.size() - 4) == ".tga") {
      stbi_write_tga(file_path.c_str(), width_, height_, 4, byte_pixels.data());
    } else if (file_path.size() >= 4 &&
               file_path.substr(file_path.size() - 4) == ".jpg") {
      stbi_write_jpg(file_path.c_str(), width_, height_, 4, byte_pixels.data(),
                     100);
    } else {
      return -1;
    }
  }
  return 0;
}

glm::vec4 &Texture::operator()(int x, int y) {
  return pixels_[y * width_ + x];
}

const glm::vec4 &Texture::operator()(int x, int y) const {
  return pixels_[y * width_ + x];
}

glm::vec4 Texture::Fetch(int x, int y, AddressMode address_mode) const {
  if (x < 0 || x >= static_cast<int>(width_) || y < 0 ||
      y >= static_cast<int>(height_)) {
    switch (address_mode) {
      case AddressMode::Repeat:
        x = ((x % static_cast<int>(width_)) + static_cast<int>(width_)) %
            static_cast<int>(width_);
        y = ((y % static_cast<int>(height_)) + static_cast<int>(height_)) %
            static_cast<int>(height_);
        break;
      case AddressMode::ClampToEdge:
        x = glm::clamp(x, 0, static_cast<int>(width_) - 1);
        y = glm::clamp(y, 0, static_cast<int>(height_) - 1);
        break;
      case AddressMode::MirrorRepeat:
        x = (x % (static_cast<int>(width_) * 2) +
             static_cast<int>(width_) * 2) %
            (static_cast<int>(width_) * 2);
        y = (y % (static_cast<int>(height_) * 2) +
             static_cast<int>(height_) * 2) %
            (static_cast<int>(height_) * 2);
        x = x >= static_cast<int>(width_) ? static_cast<int>(width_) * 2 - 1 - x
                                          : x;
        y = y >= static_cast<int>(height_)
                ? static_cast<int>(height_) * 2 - 1 - y
                : y;
        break;
      case AddressMode::BlackBorder:
        return glm::vec4{0.0f, 0.0f, 0.0f, 1.0f};
      case AddressMode::WhiteBorder:
        return glm::vec4{1.0f};
    }
  }
  return pixels_[y * width_ + x];
}

glm::vec4 Texture::Sample(float u, float v, AddressMode address_mode) const {
  //        LogInfo("Sampling: {} {}", u, v);
  u *= static_cast<float>(width_);
  v *= static_cast<float>(height_);
  u -= 0.5f;
  v -= 0.5f;
  int x, y;
  float rx, ry;
  x = static_cast<int>(std::floor(u));
  y = static_cast<int>(std::floor(v));
  rx = u - x;
  ry = v - y;
  return Fetch(x, y, address_mode) * (1.0f - rx) * (1.0f - ry) +
         Fetch(x + 1, y, address_mode) * rx * (1.0f - ry) +
         Fetch(x, y + 1, address_mode) * (1.0f - rx) * ry +
         Fetch(x + 1, y + 1, address_mode) * rx * ry;
}

uint8_t FloatToByte(float value, LDRColorSpace ldr_color_space) {
  switch (ldr_color_space) {
    case LDRColorSpace::SRGB:
      value = std::pow(value, 1.0f / 2.2f);
      break;
    default:
      break;
  }
  return static_cast<uint8_t>(
      glm::clamp(static_cast<int>(value * 255.0f), 0, 255));
}

std::vector<uint8_t> ConvertTexture(const Texture &texture,
                                    LDRColorSpace ldr_color_space) {
  std::vector<uint8_t> result(texture.Width() * texture.Height() * 4);
  for (int i = 0; i < texture.Width() * texture.Height(); i++) {
    result[i * 4] = FloatToByte(texture.Data()[i].r, ldr_color_space);
    result[i * 4 + 1] = FloatToByte(texture.Data()[i].g, ldr_color_space);
    result[i * 4 + 2] = FloatToByte(texture.Data()[i].b, ldr_color_space);
    result[i * 4 + 3] = FloatToByte(texture.Data()[i].a, ldr_color_space);
  }
  return result;
}

glm::vec4 SampleSkyBox(const std::vector<const Texture *> &sky_box,
                       const glm::vec3 &direction) {
  if (direction.y < 0.0f) {
    return glm::vec4{1.0f};
  }
  int dim = 0;
  int sig = direction.x < 0 ? -1 : 1;
  float max_val = fabs(direction.x);
  if (fabs(direction.y) > max_val) {
    dim = 1;
    sig = direction.y < 0 ? -1 : 1;
    max_val = fabs(direction.y);
  }
  if (fabs(direction.z) > max_val) {
    dim = 2;
    sig = direction.z < 0 ? -1 : 1;
    max_val = fabs(direction.z);
  }
  glm::vec3 uvw = direction / max_val;
  const float scale = 2.0f;
  if (dim == 0) {
    if (sig > 0) {
      glm::vec2 uv{uvw.z, -uvw.y};
      uv = uv * 0.5f + 0.5f;
      uv.y *= scale;
      return sky_box[1]->Sample(uv.x, uv.y);
    } else {
      glm::vec2 uv{-uvw.z, -uvw.y};
      uv = uv * 0.5f + 0.5f;
      uv.y *= scale;
      return sky_box[3]->Sample(uv.x, uv.y);
    }
  } else if (dim == 1) {
    if (sig > 0) {
      glm::vec2 uv{uvw.x, -uvw.z};
      uv = uv * 0.5f + 0.5f;
      return sky_box[4]->Sample(uv.x, uv.y);
    } else {
      return glm::vec4{1.0f};
    }
  } else {
    if (sig > 0) {
      glm::vec2 uv{-uvw.x, -uvw.y};
      uv = uv * 0.5f + 0.5f;
      uv.y *= scale;
      return sky_box[2]->Sample(uv.x, uv.y);
    } else {
      glm::vec2 uv{uvw.x, -uvw.y};
      uv = uv * 0.5f + 0.5f;
      uv.y *= scale;
      return sky_box[0]->Sample(uv.x, uv.y);
    }
  }
}

Texture SkyBoxToEnvmap(const std::vector<const Texture *> &sky_box,
                       int height) {
  Texture result{static_cast<uint32_t>(height * 2),
                 static_cast<uint32_t>(height)};
  float pi_inv_height = glm::pi<float>() / static_cast<float>(height);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < height * 2; x++) {
      float theta = pi_inv_height * (y + 0.5f);
      float phi = pi_inv_height * (x + 0.5f);
      glm::vec3 direction{-glm::sin(phi) * glm::sin(theta), glm::cos(theta),
                          glm::cos(phi) * glm::sin(theta)};
      result(x, y) = SampleSkyBox(sky_box, direction);
    }
  }
  return result;
}
}  // namespace sparkium
