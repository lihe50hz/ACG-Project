#version 450

struct AppGuiInfo {
  uvec2 selected_instance_ids;
  uvec2 hovering_instance_ids;
};

layout(set = 0, binding = 0, rg32ui) uniform uimage2D stencil_image;

layout(set = 0, binding = 1) uniform AppGuiInfoBuffer {
  AppGuiInfo app_gui_info;
};

layout(location = 0) out vec4 out_color;

void main() {
  uint hovering_instance = app_gui_info.hovering_instance_ids.x;
  uint selected_instance = app_gui_info.selected_instance_ids.x;
  ivec2 frag_pos = ivec2(gl_FragCoord.xy);
  ivec2 stencil_sz = imageSize(stencil_image);
  ivec2 d[4] = {ivec2(0, 1), ivec2(1, 0), ivec2(0, -1), ivec2(-1, 0)};
  uvec2 stencil = imageLoad(stencil_image, frag_pos).rg;
  out_color = vec4(0.0f);

  if (selected_instance != 0xfffffffeu && stencil.x != selected_instance) {
    out_color = vec4(0.0f, 0.0f, 0.0f, 0.5f);
  }

  if (stencil.x == hovering_instance && stencil.x != selected_instance) {
    for (int i = 0; i < 4; i++) {
      ivec2 load_pos = frag_pos + d[i];
      if (load_pos.x >= 0 && load_pos.y < stencil_sz.x && load_pos.y >= 0 &&
          load_pos.y < stencil_sz.y) {
        uvec2 neighbor_stencil = imageLoad(stencil_image, load_pos).rg;
        if (neighbor_stencil.x != hovering_instance) {
          out_color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
          break;
        }
      }
    }
  }
}
