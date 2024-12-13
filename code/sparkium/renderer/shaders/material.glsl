#ifndef MATERIAL_GLSL
#define MATERIAL_GLSL

#define MATERIAL_TYPE_LAMBERTIAN 0
#define MATERIAL_TYPE_SPECULAR 1
#define MATERIAL_TYPE_PRINCIPLED 2
#define MATERIAL_TYPE_VOLUMETRIC 3
#define MATERIAL_TYPE_POINTLIGHT 4

#define VOLUMETRIC_TYPE_HOMOGENEOUS 0
#define VOLUMETRIC_TYPE_CENTRALIZED 1

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

  vec3 l_e;
  float l_e_strength;
  vec3 sigma_a;
  float g;
  vec3 sigma_s;

  uint volumetric_type;
  vec3 volumetric_center;
  float center_decay;
  
  vec3 velocity;
  vec3 center;
};

#endif
