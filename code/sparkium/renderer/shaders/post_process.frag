#version 450
#include "scene_settings.glsl"

layout(set = 0, binding = 0, std140) uniform SceneSettingsUniform {
  SceneSettings scene_settings;
};

layout(input_attachment_index = 0,
       set = 1,
       binding = 0) uniform subpassInput gbuffer_radiance;

layout(location = 0) out vec4 out_color;

void main() {
  vec3 radiance = subpassLoad(gbuffer_radiance).rgb;
  out_color = vec4(pow(radiance * scene_settings.exposure,
                       vec3(1.0f / scene_settings.gamma)),
                   1.0);
}
