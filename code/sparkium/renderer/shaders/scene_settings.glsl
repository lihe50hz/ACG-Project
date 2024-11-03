#ifndef SCENE_SETTINGS_H
#define SCENE_SETTINGS_H

struct SceneSettings {
  mat4 projection;
  mat4 inv_projection;
  mat4 view;
  mat4 inv_view;
  float gamma;
  float exposure;
  float persistence;
  uint accumulated_sample;
  uint num_sample;
  uint num_bounces;
  float clamp_value;
  float total_emission_energy;
  uint num_entity;
  bool enable_direct_lighting;
};

#endif
