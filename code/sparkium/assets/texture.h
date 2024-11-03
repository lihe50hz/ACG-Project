#pragma once

#include "sparkium/utils/utils.h"

namespace sparkium {
enum class AddressMode {
  Repeat,
  ClampToEdge,
  MirrorRepeat,
  BlackBorder,
  WhiteBorder
};

enum class LDRColorSpace { SRGB, UNORM };

class Texture {
 public:
  Texture(uint32_t width = 1,
          uint32_t height = 1,
          const glm::vec4 &color = glm::vec4{1.0f});

  Texture(uint32_t width,
          uint32_t height,
          const std::vector<glm::vec4> &pixels);

  int LoadFromFile(const std::string &file_path,
                   LDRColorSpace ldr_color_space = LDRColorSpace::SRGB);

  int SaveToFile(const std::string &file_path,
                 LDRColorSpace ldr_color_space = LDRColorSpace::SRGB) const;

  uint32_t Width() const {
    return width_;
  }

  uint32_t Height() const {
    return height_;
  }

  glm::vec4 *Data() {
    return pixels_.data();
  }

  const glm::vec4 *Data() const {
    return pixels_.data();
  }

  glm::vec4 &operator()(int x, int y);

  const glm::vec4 &operator()(int x, int y) const;

  glm::vec4 Fetch(int x,
                  int y,
                  AddressMode address_mode = AddressMode::ClampToEdge) const;

  glm::vec4 Sample(float u,
                   float v,
                   AddressMode address_mode =
                       AddressMode::ClampToEdge) const;  // bilinear sampling

 private:
  std::vector<glm::vec4> pixels_;
  uint32_t width_{};
  uint32_t height_{};
};

uint8_t FloatToByte(float value, LDRColorSpace ldr_color_space);

std::vector<uint8_t> ConvertTexture(const Texture &texture,
                                    LDRColorSpace ldr_color_space);

glm::vec4 SampleSkyBox(const std::vector<const Texture *> &sky_box,
                       const glm::vec3 &direction);

Texture SkyBoxToEnvmap(const std::vector<const Texture *> &sky_box, int height);
}  // namespace sparkium
