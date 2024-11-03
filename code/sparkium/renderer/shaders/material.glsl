#ifndef MATERIAL_GLSL
#define MATERIAL_GLSL

#define MATERIAL_TYPE_LAMBERTIAN 0
#define MATERIAL_TYPE_SPECULAR 1
#define MATERIAL_TYPE_PRINCIPLED 2

struct Material {
  vec3 base_color;
  float subsurface;

  vec3 subsurface_radius;
  float metallic;

  vec3 subsurface_color;
  float specular;

  float specular_tint;
  float roughness;
  float anisotropic;
  float anisotropic_rotation;

  float sheen;
  float sheen_tint;
  float clearcoat;
  float clearcoat_roughness;

  float ior;
  float transmission;
  float transmission_roughness;
  float emission_strength;

  vec3 emission;
  float alpha;

  vec3 normal;
  uint type;
};

#endif
