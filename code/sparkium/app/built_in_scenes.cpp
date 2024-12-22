#include "sparkium/app/built_in_scenes.h"

#include "sparkium/asset_manager/asset_manager.h"
#include "sparkium/renderer/renderer.h"

namespace sparkium {

std::vector<std::pair<std::string, std::function<void(Scene *scene)>>>
BuiltInSceneList() {
  return {
      {"Living Room Scene", LoadLivingScene},
      {"Cornell Box", LoadCornellBox},
  };
}

void LoadCornellBox(Scene *scene) {
  AssetManager *asset_manager = scene->Renderer()->AssetManager();

  auto make_vertex = [](const glm::vec3 &pos, const glm::vec2 &tex_coord) {
    Vertex vertex;
    vertex.position = pos;
    vertex.tex_coord = tex_coord;
    return vertex;
  };

  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices = {0, 1, 3, 1, 2, 3};

  // light
  // <vertex position="343.0 548.7 227.0" tex_coord="0 0"/>
  // <vertex position="343.0 548.7 332.0" tex_coord="1 0"/>
  // <vertex position="213.0 548.7 332.0" tex_coord="1 1"/>
  // <vertex position="213.0 548.7 227.0" tex_coord="0 1"/>

  vertices.push_back(make_vertex({343.0f, 548.7f, 227.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({343.0f, 548.7f, 332.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({213.0f, 548.7f, 332.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({213.0f, 548.7f, 227.0f}, {0.0f, 1.0f}));
  int light_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "LightMesh");
  Material light_material;
  light_material.base_color = {0.0f, 0.0f, 0.0f};
  light_material.emission = {1.0f, 1.0f, 1.0f};
  light_material.emission_strength = 100.0f;
  int light_id = scene->CreateEntity();
  scene->SetEntityMesh(light_id, light_mesh_id);
  scene->SetEntityMaterial(light_id, light_material);

  // floor
  // <vertex position="552.8 0.0   0.0" tex_coord="0 0"/>
  // <vertex position="0.0 0.0   0.0" tex_coord="1 0"/>
  // <vertex position="0.0 0.0 559.2" tex_coord="1 1"/>
  // <vertex position="549.6 0.0 559.2" tex_coord="0 1"/>
  vertices.clear();
  vertices.push_back(make_vertex({552.8f, 0.0f, 0.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 0.0f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({549.6f, 0.0f, 559.2f}, {0.0f, 1.0f}));
  int floor_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "FloorMesh");
  Material floor_material;
  floor_material.base_color = {0.8f, 0.8f, 0.8f};
  int floor_id = scene->CreateEntity();
  scene->SetEntityMesh(floor_id, floor_mesh_id);
  scene->SetEntityMaterial(floor_id, floor_material);

  // ceiling
  // <vertex position="556.0 548.8 0.0  " tex_coord="0 0"/>
  // <vertex position="556.0 548.8 559.2" tex_coord="1 0"/>
  // <vertex position="0.0 548.8 559.2" tex_coord="1 1"/>
  // <vertex position="0.0 548.8   0.0" tex_coord="0 1"/>
  vertices.clear();
  vertices.push_back(make_vertex({556.0f, 548.8f, 0.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({556.0f, 548.8f, 559.2f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 548.8f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({0.0f, 548.8f, 0.0f}, {0.0f, 1.0f}));
  int ceiling_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "CeilingMesh");
  Material ceiling_material;
  ceiling_material.base_color = {0.8f, 0.8f, 0.8f};
  int ceiling_id = scene->CreateEntity();
  scene->SetEntityMesh(ceiling_id, ceiling_mesh_id);
  scene->SetEntityMaterial(ceiling_id, ceiling_material);

  // back_wall
  // <vertex position="549.6   0.0 559.2" tex_coord="0 0"/>
  // <vertex position="0.0   0.0 559.2" tex_coord="1 0"/>
  // <vertex position="0.0 548.8 559.2" tex_coord="1 1"/>
  // <vertex position="556.0 548.8 559.2" tex_coord="0 1"/>
  vertices.clear();
  vertices.push_back(make_vertex({549.6f, 0.0f, 559.2f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 0.0f, 559.2f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 548.8f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({556.0f, 548.8f, 559.2f}, {0.0f, 1.0f}));
  int back_wall_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "BackWallMesh");
  Material back_wall_material;
  back_wall_material.base_color = {0.8f, 0.8f, 0.8f};
  int back_wall_id = scene->CreateEntity();
  scene->SetEntityMesh(back_wall_id, back_wall_mesh_id);
  scene->SetEntityMaterial(back_wall_id, back_wall_material);

  // right_wall
  // <vertex position="0.0   0.0 559.2 " tex_coord="0 0"/>
  // <vertex position = "0.0   0.0   0.0" tex_coord = "1 0" />
  // <vertex position = "0.0 548.8   0.0" tex_coord = "1 1" />
  // <vertex position = "0.0 548.8 559.2" tex_coord = "0 1" />
  vertices.clear();
  vertices.push_back(make_vertex({0.0f, 0.0f, 559.2f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({0.0f, 548.8f, 0.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({0.0f, 548.8f, 559.2f}, {0.0f, 1.0f}));
  int right_wall_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "RightWallMesh");
  Material right_wall_material;
  right_wall_material.base_color = {0.0, 0.8, 0.0};
  int right_wall_id = scene->CreateEntity();
  scene->SetEntityMesh(right_wall_id, right_wall_mesh_id);
  scene->SetEntityMaterial(right_wall_id, right_wall_material);

  // left_wall
  // <vertex position="552.8   0.0   0.0 " tex_coord="0 0"/>
  // <vertex position = "549.6   0.0 559.2" tex_coord = "1 0" />
  // <vertex position = "556.0 548.8 559.2" tex_coord = "1 1" />
  // <vertex position = "556.0 548.8   0.0" tex_coord = "0 1" />
  vertices.clear();
  vertices.push_back(make_vertex({552.8f, 0.0f, 0.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({549.6f, 0.0f, 559.2f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({556.0f, 548.8f, 559.2f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({556.0f, 548.8f, 0.0f}, {0.0f, 1.0f}));
  int left_wall_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "LeftWallMesh");
  Material left_wall_material;
  left_wall_material.base_color = {0.8f, 0.0f, 0.0f};
  int left_wall_id = scene->CreateEntity();
  scene->SetEntityMesh(left_wall_id, left_wall_mesh_id);
  scene->SetEntityMaterial(left_wall_id, left_wall_material);

  // short_box
  // <vertex position="130.0 165.0  65.0" tex_coord="0 0"/>
  // <vertex position = "82.0 165.0 225.0" tex_coord = "1 0" />
  // <vertex position = "240.0 165.0 272.0" tex_coord = "1 1" />
  // <vertex position = "290.0 165.0 114.0" tex_coord = "0 1" />
  //
  // <vertex position = "290.0   0.0 114.0" tex_coord = "0 0" />
  // <vertex position = "290.0 165.0 114.0" tex_coord = "1 0" />
  // <vertex position = "240.0 165.0 272.0" tex_coord = "1 1" />
  // <vertex position = "240.0   0.0 272.0" tex_coord = "0 1" />
  //
  // <vertex position = "130.0   0.0  65.0" tex_coord = "0 0" />
  // <vertex position = "130.0 165.0  65.0" tex_coord = "1 0" />
  // <vertex position = "290.0 165.0 114.0" tex_coord = "1 1" />
  // <vertex position = "290.0   0.0 114.0" tex_coord = "0 1" />
  //
  // <vertex position = "82.0   0.0 225.0" tex_coord = "0 0" />
  // <vertex position = "82.0 165.0 225.0" tex_coord = "1 0" />
  // <vertex position = "130.0 165.0  65.0" tex_coord = "1 1" />
  // <vertex position = "130.0   0.0  65.0" tex_coord = "0 1" />
  //
  // <vertex position = "240.0   0.0 272.0" tex_coord = "0 0" />
  // <vertex position = "240.0 165.0 272.0" tex_coord = "1 0" />
  // <vertex position = " 82.0 165.0 225.0" tex_coord = "1 1" />
  // <vertex position = "82.0   0.0 225.0" tex_coord = "0 1" />
  //
  // <index value = "0" /><index value = "1" /><index value = "3" />
  // <index value = "1" /><index value = "2" /><index value = "3" />
  // <index value = "4" /><index value = "5" /><index value = "7" />
  // <index value = "5" /><index value = "6" /><index value = "7" />
  // <index value = "8" /><index value = "9" /><index value = "11" />
  // <index value = "9" /><index value = "10" /><index value = "11" />
  // <index value = "12" /><index value = "13" /><index value = "15" />
  // <index value = "13" /><index value = "14" /><index value = "15" />
  // <index value = "16" /><index value = "17" /><index value = "19" />
  // <index value = "17" /><index value = "18" /> <index value = "19" />

  /*indices = {0, 1,  3,  1,  2,  3,  4,  5,  7,  5,  6,  7,  8,  9,  11,
             9, 10, 11, 12, 13, 15, 13, 14, 15, 16, 17, 19, 17, 18, 19, 20, 21,
  23, 21, 22, 23}; vertices.clear(); vertices.push_back(make_vertex({130.0f,
  165.0f, 65.0f}, {0.0f, 0.0f})); vertices.push_back(make_vertex({82.0f, 165.0f,
  225.0f}, {1.0f, 0.0f})); vertices.push_back(make_vertex({240.0f, 165.0f,
  272.0f}, {1.0f, 1.0f})); vertices.push_back(make_vertex({290.0f, 165.0f,
  114.0f}, {0.0f, 1.0f})); vertices.push_back(make_vertex({290.0f, 0.0f,
  114.0f}, {0.0f, 0.0f})); vertices.push_back(make_vertex({290.0f, 165.0f,
  114.0f}, {1.0f, 0.0f})); vertices.push_back(make_vertex({240.0f, 165.0f,
  272.0f}, {1.0f, 1.0f})); vertices.push_back(make_vertex({240.0f, 0.0f,
  272.0f}, {0.0f, 1.0f})); vertices.push_back(make_vertex({130.0f, 0.0f, 65.0f},
  {0.0f, 0.0f})); vertices.push_back(make_vertex({130.0f, 165.0f, 65.0f}, {1.0f,
  0.0f})); vertices.push_back(make_vertex({290.0f, 165.0f, 114.0f},
  {1.0f, 1.0f})); vertices.push_back(make_vertex({290.0f, 0.0f, 114.0f},
  {0.0f, 1.0f})); vertices.push_back(make_vertex({82.0f, 0.0f, 225.0f}, {0.0f,
  0.0f})); vertices.push_back(make_vertex({82.0f, 165.0f, 225.0f}, {1.0f,
  0.0f})); vertices.push_back(make_vertex({130.0f, 165.0f, 65.0f},
  {1.0f, 1.0f})); vertices.push_back(make_vertex({130.0f, 0.0f, 65.0f},
  {0.0f, 1.0f})); vertices.push_back(make_vertex({240.0f, 0.0f, 272.0f}, {0.0f,
  0.0f})); vertices.push_back(make_vertex({240.0f, 165.0f, 272.0f}, {1.0f,
  0.0f})); vertices.push_back(make_vertex({82.0f, 165.0f, 225.0f},
  {1.0f, 1.0f})); vertices.push_back(make_vertex({82.0f, 0.0f, 225.0f},
  {0.0f, 1.0f})); vertices.push_back(make_vertex({130.0f, 0.0f, 65.0f}, {0.0f,
  0.0f})); vertices.push_back(make_vertex({82.0f, 0.0f, 225.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({240.0f, 0.0f, 272.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({290.0f, 0.0f, 114.0f}, {0.0f, 1.0f}));
  int short_box_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "ShortBoxMesh");
  Material short_box_material;
  short_box_material.base_color = {0.8f, 0.8f, 0.8f};
  int short_box_id = scene->CreateEntity();
  scene->SetEntityMesh(short_box_id, short_box_mesh_id);
  scene->SetEntityMaterial(short_box_id, short_box_material);*/

  // box 2
  /*indices = {0,  1,  3,  1,  2,  3,  4,  5,  7,  5,  6,  7,
             8,  9,  11, 9,  10, 11, 12, 13, 15, 13, 14, 15,
             16, 17, 19, 17, 18, 19, 20, 21, 23, 21, 22, 23};
  vertices.clear();
  vertices.push_back(make_vertex({130.0f, 365.0f, 65.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({82.0f, 365.0f, 225.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({240.0f, 365.0f, 272.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({290.0f, 365.0f, 114.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({290.0f, 200.0f, 114.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({290.0f, 365.0f, 114.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({240.0f, 365.0f, 272.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({240.0f, 200.0f, 272.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({130.0f, 200.0f, 65.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({130.0f, 365.0f, 65.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({290.0f, 365.0f, 114.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({290.0f, 200.0f, 114.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({82.0f, 200.0f, 225.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({82.0f, 365.0f, 225.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({130.0f, 365.0f, 65.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({130.0f, 200.0f, 65.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({240.0f, 200.0f, 272.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({240.0f, 365.0f, 272.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({82.0f, 365.0f, 225.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({82.0f, 200.0f, 225.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({130.0f, 200.0f, 65.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({82.0f, 200.0f, 225.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({240.0f, 200.0f, 272.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({290.0f, 200.0f, 114.0f}, {0.0f, 1.0f}));
  int short_box2_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "ShortBoxMesh");
  Material short_box2_material;
  short_box2_material.base_color = {0.8f, 0.8f, 0.8f};
  int short_box2_id = scene->CreateEntity();
  scene->SetEntityMesh(short_box2_id, short_box2_mesh_id);
  scene->SetEntityMaterial(short_box2_id, short_box2_material);*/

  // tall_box
  // <vertex position="423.0 330.0 247.0" tex_coord="0 0"/>
  // <vertex position="265.0 330.0 296.0" tex_coord="1 0"/>
  // <vertex position="314.0 330.0 456.0" tex_coord="1 1"/>
  // <vertex position="472.0 330.0 406.0" tex_coord="0 1"/>
  //
  // <vertex position="423.0   0.0 247.0" tex_coord="0 0"/>
  // <vertex position="423.0 330.0 247.0" tex_coord="1 0"/>
  // <vertex position="472.0 330.0 406.0" tex_coord="1 1"/>
  // <vertex position="472.0   0.0 406.0" tex_coord="0 1"/>
  //
  // <vertex position="472.0   0.0 406.0" tex_coord="0 0"/>
  // <vertex position="472.0 330.0 406.0" tex_coord="1 0"/>
  // <vertex position="314.0 330.0 456.0" tex_coord="1 1"/>
  // <vertex position="314.0   0.0 456.0" tex_coord="0 1"/>
  //
  // <vertex position="314.0   0.0 456.0" tex_coord="0 0"/>
  // <vertex position="314.0 330.0 456.0" tex_coord="1 0"/>
  // <vertex position="265.0 330.0 296.0" tex_coord="1 1"/>
  // <vertex position="265.0   0.0 296.0" tex_coord="0 1"/>
  //
  // <vertex position="265.0   0.0 296.0" tex_coord="0 0"/>
  // <vertex position="265.0 330.0 296.0" tex_coord="1 0"/>
  // <vertex position="423.0 330.0 247.0" tex_coord="1 1"/>
  // <vertex position="423.0   0.0 247.0" tex_coord="0 1"/>
  //
  //
  // <index value="0"/>
  // <index value="1"/>
  // <index value="3"/>
  // <index value="1"/>
  // <index value="2"/>
  // <index value="3"/>
  // <index value="4"/>
  // <index value="5"/>
  // <index value="7"/>
  // <index value="5"/>
  // <index value="6"/>
  // <index value="7"/>
  // <index value="8"/>
  // <index value="9"/>
  // <index value="11"/>
  // <index value="9"/>
  // <index value="10"/>
  // <index value="11"/>
  // <index value="12"/>
  // <index value="13"/>
  // <index value="15"/>
  // <index value="13"/>
  // <index value="14"/>
  // <index value="15"/>
  // <index value="16"/>
  // <index value="17"/>
  // <index value="19"/>
  // <index value="17"/>
  // <index value="18"/>
  // <index value="19"/>

  /*indices = {0, 1,  3,  1,  2,  3,  4,  5,  7,  5,  6,  7,  8,  9,  11,
             9, 10, 11, 12, 13, 15, 13, 14, 15, 16, 17, 19, 17, 18, 19, 20, 21,
  23, 21, 22, 23}; vertices.clear(); vertices.push_back(make_vertex({423.0f,
  330.0f, 247.0f}, {0.0f, 0.0f})); vertices.push_back(make_vertex({265.0f,
  330.0f, 296.0f}, {1.0f, 0.0f})); vertices.push_back(make_vertex({314.0f,
  330.0f, 456.0f}, {1.0f, 1.0f})); vertices.push_back(make_vertex({472.0f,
  330.0f, 406.0f}, {0.0f, 1.0f})); vertices.push_back(make_vertex({423.0f, 0.0f,
  247.0f}, {0.0f, 0.0f})); vertices.push_back(make_vertex({423.0f, 330.0f,
  247.0f}, {1.0f, 0.0f})); vertices.push_back(make_vertex({472.0f, 330.0f,
  406.0f}, {1.0f, 1.0f})); vertices.push_back(make_vertex({472.0f, 0.0f,
  406.0f}, {0.0f, 1.0f})); vertices.push_back(make_vertex({472.0f, 0.0f,
  406.0f}, {0.0f, 0.0f})); vertices.push_back(make_vertex({472.0f, 330.0f,
  406.0f}, {1.0f, 0.0f})); vertices.push_back(make_vertex({314.0f, 330.0f,
  456.0f}, {1.0f, 1.0f})); vertices.push_back(make_vertex({314.0f, 0.0f,
  456.0f}, {0.0f, 1.0f})); vertices.push_back(make_vertex({314.0f, 0.0f,
  456.0f}, {0.0f, 0.0f})); vertices.push_back(make_vertex({314.0f, 330.0f,
  456.0f}, {1.0f, 0.0f})); vertices.push_back(make_vertex({265.0f, 330.0f,
  296.0f}, {1.0f, 1.0f})); vertices.push_back(make_vertex({265.0f, 0.0f,
  296.0f}, {0.0f, 1.0f})); vertices.push_back(make_vertex({265.0f, 0.0f,
  296.0f}, {0.0f, 0.0f})); vertices.push_back(make_vertex({265.0f, 330.0f,
  296.0f}, {1.0f, 0.0f})); vertices.push_back(make_vertex({423.0f, 330.0f,
  247.0f}, {1.0f, 1.0f})); vertices.push_back(make_vertex({423.0f, 0.0f,
  247.0f}, {0.0f, 1.0f})); vertices.push_back(make_vertex({423.0f, 0.0f,
  247.0f}, {0.0f, 0.0f})); vertices.push_back(make_vertex({265.0f, 0.0f,
  296.0f}, {1.0f, 0.0f})); vertices.push_back(make_vertex({314.0f, 0.0f,
  456.0f}, {1.0f, 1.0f})); vertices.push_back(make_vertex({472.0f, 0.0f,
  406.0f}, {0.0f, 1.0f}));

  int tall_box_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "TallBoxMesh");
  Material tall_box_material;
  tall_box_material.base_color = {0.8f, 0.8f, 0.8f};
  int tall_box_id = scene->CreateEntity();
  scene->SetEntityMesh(tall_box_id, tall_box_mesh_id);
  scene->SetEntityMaterial(tall_box_id, tall_box_material);*/
  /*
  Mesh longma;
  longma.LoadObjFile(FindAssetsFile("mesh/longma.obj"));
  int longma_mesh_id = asset_manager->LoadMesh(longma, "LongMa");

  Material longma_material;
  
  longma_material.base_color = {1.0f, 1.0f, 1.0f};
  int longma_id = scene->CreateEntity();
  scene->SetEntityMesh(longma_id, longma_mesh_id);
  scene->SetEntityMaterial(longma_id, longma_material);
  Texture longma_texture;
  longma_texture.LoadFromFile(FindAssetsFile("texture/living/LMspirit.jpg"),
                                  LDRColorSpace::UNORM);
  auto longma_texture_id =
      asset_manager->LoadTexture(longma_texture, "LMTexture");
  scene->SetEntityAlbedoTexture(longma_id, longma_texture_id);

  Mesh longma2;
  longma2.LoadObjFile(FindAssetsFile("mesh/longma2.obj"));
  int longma2_mesh_id = asset_manager->LoadMesh(longma2, "LongMa2");

  Material longma2_material;

  longma2_material.base_color = {1.0f, 1.0f, 1.0f};
  int longma2_id = scene->CreateEntity();
  scene->SetEntityMesh(longma2_id, longma2_mesh_id);
  scene->SetEntityMaterial(longma2_id, longma2_material);

  Mesh longma3;
  longma3.LoadObjFile(FindAssetsFile("mesh/longma3.obj"));
  int longma3_mesh_id = asset_manager->LoadMesh(longma3, "LongMa3");

  Material longma3_material;

  longma3_material.base_color = {1.0f, 1.0f, 1.0f};
  int longma3_id = scene->CreateEntity();
  scene->SetEntityMesh(longma3_id, longma3_mesh_id);
  scene->SetEntityMaterial(longma3_id, longma3_material);
  Texture longma3_texture;
  longma3_texture.LoadFromFile(FindAssetsFile("texture/living/Wood09_diffuse_xtm.jpg"),
                              LDRColorSpace::UNORM);
  auto longma3_texture_id =
      asset_manager->LoadTexture(longma3_texture, "LMTexture3");
  scene->SetEntityAlbedoTexture(longma3_id, longma3_texture_id);

  scene->SetEntityTransform(
      longma_id,
      glm::translate(glm::mat4{1.0f},
                     glm::vec3{0.0f, 0.0f, 400.0f}) *    // Z����ƽ��+400
          glm::scale(glm::mat4{1.0f}, glm::vec3{40.0f})  // �Ŵ�40��
  );
  scene->SetEntityTransform(
      longma2_id,
      glm::translate(glm::mat4{1.0f},
                     glm::vec3{0.0f, 0.0f, 400.0f}) *    // Z����ƽ��+400
          glm::scale(glm::mat4{1.0f}, glm::vec3{40.0f})  // �Ŵ�40��
  );
  scene->SetEntityTransform(
      longma3_id,
      glm::translate(glm::mat4{1.0f},
                     glm::vec3{0.0f, 0.0f, 400.0f}) *    // Z����ƽ��+400
          glm::scale(glm::mat4{1.0f}, glm::vec3{40.0f})  // �Ŵ�40��
  );

  */
  //Mesh test, testt;
  //test.LoadObjFile(FindAssetsFile("mesh/cube.obj"));
  //test.ThisIsPointLight();
  
  //int test_mesh_id = asset_manager->LoadMesh(test, "Cube");

  //Material test_material;
  //test_material.base_color = {0.0f, 0.0f, 0.8f};
  //test_material.emission = {1.0f, 1.0f, 1.0f};
  //test_material.emission_strength = 10.0f;
  //test_material.type = MATERIAL_TYPE_POINTLIGHT;  // Point Light
  //test_material.center = {60.0f, 105.0f, 80.0f};
  //int test_id = scene->CreateEntity();
  //scene->SetEntityMesh(test_id, test_mesh_id);
  //scene->SetEntityMaterial(test_id, test_material);

  //testt.LoadObjFile(FindAssetsFile("mesh/cube.obj"));
  //testt.ThisIsPointLight(glm::vec3{131.452f, -114.514f, -191.981f});
  //int testt_mesh_id = asset_manager->LoadMesh(testt, "Cube");
  //Material testt_material;
  //testt_material.base_color = {0.0f, 0.0f, 0.8f};
  //testt_material.emission = {1.0f, 1.0f, 1.0f};
  //testt_material.emission_strength = 10.0f;
  //testt_material.type = MATERIAL_TYPE_POINTLIGHT;  // Point Light
  //testt_material.center = {60.0f, 105.0f, 100.0f};
  //int testt_id = scene->CreateEntity();
  //scene->SetEntityMesh(testt_id, testt_mesh_id);
  //scene->SetEntityMaterial(testt_id, testt_material);

  Texture milkdragon_texture;
  milkdragon_texture.LoadFromFile(FindAssetsFile("texture/IIIS.png"),
                                  LDRColorSpace::UNORM);
  auto milkdragon_texture_id =
      asset_manager->LoadTexture(milkdragon_texture, "IIISTexture");
  scene->SetEntityAlbedoTexture(back_wall_id, milkdragon_texture_id);
  // scene->SetEntityAlbedoDetailTexture(back_wall_id, milkdragon_texture_id);

  Texture thu_texture;
  thu_texture.LoadFromFile(FindAssetsFile("texture/living/white_tiles_2_diffuse.jpg"),
                           LDRColorSpace::UNORM);
  auto thu_texture_id = asset_manager->LoadTexture(thu_texture, "thuTexture");
  scene->SetEntityAlbedoTexture(back_wall_id, thu_texture_id);
  // scene->SetEntityAlbedoDetailTexture(back_wall_id, thu_texture_id);

  Texture normal_texture;
  normal_texture.LoadFromFile(FindAssetsFile("texture/normal.png"),
                              LDRColorSpace::UNORM);
  auto normal_texture_id =
      asset_manager->LoadTexture(normal_texture, "NormalTexture");
  /*scene->SetEntityNormalTexture(back_wall_id, normal_texture_id);*/

  scene->SetEnvmapSettings({0.0f, 0.0f, 0, 0});
  /*auto envmap = scene->GetEnvMap();

  Texture envmap_texture;
  envmap_texture.LoadFromFile(FindAssetsFile("texture/envmap_clouds_4k.hdr"),
                              LDRColorSpace::UNORM);
  auto envmap_id = asset_manager->LoadTexture(envmap_texture, "Envmap");
  envmap->SetEnvmapTexture(envmap_id);
  scene->SetEnvmapSettings({0.0f, 1.0f, uint32_t(envmap_id), 0});*/

  scene->Camera()->SetPosition({278.0f, 273.0f, -800.0f});
  scene->Camera()->SetEulerAngles({0.0f, glm::radians(180.0f), 0.0f});
  scene->Camera()->SetFov(glm::radians(40.0f));
  scene->Camera()->SetFar(2000.0f);
  scene->Camera()->SetCameraSpeed(100.0f);
}

void LoadIslandScene(Scene *scene) {
  AssetManager *asset_manager = scene->Renderer()->AssetManager();
  scene->Camera()->GetPosition() = glm::vec3{0.0f, 0.0f, 5.0f};

  auto envmap = scene->GetEnvMap();

  Texture envmap_texture;
  envmap_texture.LoadFromFile(FindAssetsFile("texture/envmap_clouds_4k.hdr"),
                              LDRColorSpace::UNORM);
  auto envmap_id = asset_manager->LoadTexture(envmap_texture, "Envmap");
  envmap->SetEnvmapTexture(envmap_id);
  scene->SetEnvmapSettings({0.0f, 1.0f, uint32_t(envmap_id), 0});

  int entity_id = scene->CreateEntity();

  Texture terrain_texture;
  terrain_texture.LoadFromFile(
      FindAssetsFile("texture/terrain/terrain-texture3.bmp"),
      LDRColorSpace::UNORM);

  Texture terrain_detail_texture;
  terrain_detail_texture.LoadFromFile(
      FindAssetsFile("texture/terrain/detail.bmp"), LDRColorSpace::UNORM);
  auto terrain_texture_id =
      asset_manager->LoadTexture(terrain_texture, "TerrainTexture");
  auto terrain_detail_texture_id = asset_manager->LoadTexture(
      terrain_detail_texture, "TerrainDetailTexture");

  scene->SetEntityAlbedoTexture(entity_id, terrain_texture_id);
  scene->SetEntityAlbedoDetailTexture(entity_id, terrain_detail_texture_id);

  Texture heightmap_texture;
  heightmap_texture.LoadFromFile(
      FindAssetsFile("texture/terrain/heightmap.bmp"), LDRColorSpace::UNORM);
  Mesh terrain_mesh;
  terrain_mesh.LoadFromHeightMap(heightmap_texture, 1.0f, 0.2f, 0.0f);
  auto terrain_mesh_id = asset_manager->LoadMesh(terrain_mesh, "TerrainMesh");
  Material terrain_material;
  terrain_material.sheen = 1.0f;
  scene->SetEntityMesh(entity_id, terrain_mesh_id);
  scene->SetEntityMaterial(entity_id, terrain_material);
  scene->SetEntityTransform(
      entity_id,
      glm::translate(glm::mat4{1.0f}, glm::vec3{0.0f, -0.06f, 0.0f}));
  scene->SetEntityDetailScaleOffset(entity_id, {20.0f, 20.0f, 0.0f, 0.0f});

  Mesh plane_mesh;
  std::vector<Vertex> plane_vertices;
  std::vector<uint32_t> plane_indices;
  const int precision = 500;
  const float inv_precision = 1.0f / static_cast<float>(precision);
  for (int i = 0; i <= precision; i++) {
    for (int j = 0; j <= precision; j++) {
      Vertex vertex;
      vertex.position = {static_cast<float>(i) * inv_precision - 0.5f, 0.0f,
                         static_cast<float>(j) * inv_precision - 0.5f};
      vertex.normal = {0.0f, 1.0f, 0.0f};
      vertex.tangent = {1.0f, 0.0f, 0.0f};
      vertex.tex_coord = {static_cast<float>(i) * inv_precision,
                          1.0f - static_cast<float>(j) * inv_precision};
      vertex.signal = 1.0f;
      plane_vertices.push_back(vertex);
    }
  }
  for (int i = 0; i < precision; i++) {
    for (int j = 0; j < precision; j++) {
      plane_indices.push_back(i * (precision + 1) + j);
      plane_indices.push_back(i * (precision + 1) + j + 1);
      plane_indices.push_back((i + 1) * (precision + 1) + j);
      plane_indices.push_back((i + 1) * (precision + 1) + j);
      plane_indices.push_back(i * (precision + 1) + j + 1);
      plane_indices.push_back((i + 1) * (precision + 1) + j + 1);
    }
  }
  plane_mesh = {plane_vertices, plane_indices};

  auto plane_mesh_id = asset_manager->LoadMesh(plane_mesh, "PlaneMesh");

  Texture water_texture;
  water_texture.LoadFromFile(
      FindAssetsFile("texture/terrain/SkyBox/SkyBox5.bmp"),
      LDRColorSpace::UNORM);
  auto water_texture_id =
      asset_manager->LoadTexture(water_texture, "WaterTexture");
  int water_entity_id = scene->CreateEntity();
  Material water_material;
  water_material.base_color = {1.0f, 1.0f, 1.0f};
  water_material.metallic = 0.1f;
  water_material.specular = 1.0f;
  water_material.alpha = 1.0f;
  scene->SetEntityAlbedoDetailTexture(water_entity_id, water_texture_id);
  scene->SetEntityMesh(water_entity_id, plane_mesh_id);
  scene->SetEntityMaterial(water_entity_id, water_material);
  scene->SetEntityTransform(water_entity_id,
                            glm::scale(glm::mat4{1.0f}, glm::vec3{1000.0f}));
  scene->SetEntityDetailScaleOffset(water_entity_id,
                                    {10000.0f, 10000.0f, 0.0f, 0.0f});

  scene->Camera()->SetFar(500.0f);
  scene->Camera()->SetNear(0.05f);
  scene->Camera()->SetPosition({0.0f, 0.1f, 1.2f});

  scene->SetUpdateCallback([=](Scene *scene, float delta_time) {
    glm::vec2 speed{0.3f, 1.0f};
    speed *= delta_time * 0.1f;
    glm::vec4 detail_scale_offset;
    scene->GetEntityDetailScaleOffset(water_entity_id, detail_scale_offset);
    detail_scale_offset.z += speed.x;
    detail_scale_offset.w += speed.y;
    detail_scale_offset.z = glm::mod(detail_scale_offset.z, 1.0f);
    detail_scale_offset.w = glm::mod(detail_scale_offset.w, 1.0f);
    scene->SetEntityDetailScaleOffset(water_entity_id, detail_scale_offset);
  });
}

void LoadLivingScene(Scene *scene) {
  AssetManager *asset_manager = scene->Renderer()->AssetManager();
  scene->Camera()->GetPosition() = glm::vec3{0.0f, 0.0f, 5.0f};

  auto envmap = scene->GetEnvMap();

  Texture envmap_texture;
  envmap_texture.LoadFromFile(FindAssetsFile("texture/living/wooden_lounge_4k.hdr"),
                              LDRColorSpace::UNORM);
  auto envmap_id = asset_manager->LoadTexture(envmap_texture, "Envmap");
  envmap->SetEnvmapTexture(envmap_id);
  scene->SetEnvmapSettings({0.0f, 1.0f, uint32_t(envmap_id), 0});

  // Floor
  int floor_id = scene->CreateEntity();

  Texture floor_texture;
  floor_texture.LoadFromFile(
      FindAssetsFile("texture/living/white_tiles_2_diffuse.png"),
      LDRColorSpace::UNORM);
  auto floor_texture_id =
      asset_manager->LoadTexture(floor_texture, "FloorTexture");
  scene->SetEntityAlbedoTexture(floor_id, floor_texture_id);
  
  Texture floor_normal_texture;
  floor_normal_texture.LoadFromFile(FindAssetsFile("texture/living/white_tiles_2_normal.png"),
                              LDRColorSpace::UNORM);
  auto floor_normal_texture_id =
      asset_manager->LoadTexture(floor_normal_texture, "FloorNormalTexture");
  scene->SetEntityNormalTexture(floor_id, floor_normal_texture_id);

  Mesh floor_mesh;
  floor_mesh.LoadObjFile(FindAssetsFile("mesh/living/floor.obj"));
  auto floor_mesh_id = asset_manager->LoadMesh(floor_mesh, "FloorMesh");
  Material floor_material;
  scene->SetEntityMesh(floor_id, floor_mesh_id);
  scene->SetEntityMaterial(floor_id, floor_material);

  // Left Wall
  int leftwall_id = scene->CreateEntity();

  Texture leftwall_texture;
  leftwall_texture.LoadFromFile(
      FindAssetsFile("texture/living/black_and_white_marble_23_37_diffuse.jpg"),
      LDRColorSpace::UNORM);
  auto leftwall_texture_id =
      asset_manager->LoadTexture(leftwall_texture, "LeftWallTexture");
  scene->SetEntityAlbedoTexture(leftwall_id, leftwall_texture_id);

  Mesh leftwall_mesh;
  leftwall_mesh.LoadObjFile(FindAssetsFile("mesh/living/leftwall.obj"));
  auto leftwall_mesh_id = asset_manager->LoadMesh(leftwall_mesh, "LeftWallMesh");
  Material leftwall_material;
  scene->SetEntityMesh(leftwall_id, leftwall_mesh_id);
  scene->SetEntityMaterial(leftwall_id, leftwall_material);

  // Right Wall
  int rightwall_id = scene->CreateEntity();

  Mesh rightwall_mesh;
  rightwall_mesh.LoadObjFile(FindAssetsFile("mesh/living/rightwall.obj"));
  auto rightwall_mesh_id =
      asset_manager->LoadMesh(rightwall_mesh, "RightWallMesh");
  Material rightwall_material;
  scene->SetEntityMesh(rightwall_id, rightwall_mesh_id);
  scene->SetEntityMaterial(rightwall_id, rightwall_material);

  // Back Wall
  int backwall_id = scene->CreateEntity();

  Mesh backwall_mesh;
  backwall_mesh.LoadObjFile(FindAssetsFile("mesh/living/backwall.obj"));
  auto backwall_mesh_id =
      asset_manager->LoadMesh(backwall_mesh, "BackWallMesh");
  Material backwall_material;
  scene->SetEntityMesh(backwall_id, backwall_mesh_id);
  scene->SetEntityMaterial(backwall_id, backwall_material);

  // Ceiling
  int ceiling_id = scene->CreateEntity();

  Mesh ceiling_mesh;
  ceiling_mesh.LoadObjFile(FindAssetsFile("mesh/living/ceiling.obj"));
  auto ceiling_mesh_id =
      asset_manager->LoadMesh(ceiling_mesh, "CeilingMesh");
  Material ceiling_material;
  scene->SetEntityMesh(ceiling_id, ceiling_mesh_id);
  scene->SetEntityMaterial(ceiling_id, ceiling_material);

  // Four Chairs
  int fourchairs_id = scene->CreateEntity();

  Mesh fourchairs_mesh;
  fourchairs_mesh.LoadObjFile(FindAssetsFile("mesh/living/fourchairs.obj"));
  auto fourchairs_mesh_id =
      asset_manager->LoadMesh(fourchairs_mesh, "FourChairsMesh");
  Material fourchairs_material;
  scene->SetEntityMesh(fourchairs_id, fourchairs_mesh_id);
  scene->SetEntityMaterial(fourchairs_id, fourchairs_material);

  // Two Chairs
  int twochairs_id = scene->CreateEntity();

  Mesh twochairs_mesh;
  twochairs_mesh.LoadObjFile(FindAssetsFile("mesh/living/twochairs.obj"));
  auto twochairs_mesh_id =
      asset_manager->LoadMesh(twochairs_mesh, "TwoChairsMesh");
  scene->SetEntityMesh(twochairs_id, twochairs_mesh_id);

  Material twochairs_material;
  scene->SetEntityMaterial(twochairs_id, twochairs_material);

  // Table Face
  int tableface_id = scene->CreateEntity();

  Mesh tableface_mesh;
  tableface_mesh.LoadObjFile(FindAssetsFile("mesh/living/tableface.obj"));
  auto tableface_mesh_id =
      asset_manager->LoadMesh(tableface_mesh, "TableFaceMesh");
  scene->SetEntityMesh(tableface_id, tableface_mesh_id);

  Material tableface_material;
  scene->SetEntityMaterial(tableface_id, tableface_material);

  // Table Feet
  int tablefeet_id = scene->CreateEntity();

  Mesh tablefeet_mesh;
  tablefeet_mesh.LoadObjFile(FindAssetsFile("mesh/living/tablefeet.obj"));
  auto tablefeet_mesh_id =
      asset_manager->LoadMesh(tablefeet_mesh, "TableFeetMesh");
  scene->SetEntityMesh(tablefeet_id, tablefeet_mesh_id);

  Material tablefeet_material;
  scene->SetEntityMaterial(tablefeet_id, tablefeet_material);

  // Plate
  int plate_id = scene->CreateEntity();

  Mesh plate_mesh;
  plate_mesh.LoadObjFile(FindAssetsFile("mesh/living/plate.obj"));
  auto plate_mesh_id =
      asset_manager->LoadMesh(plate_mesh, "PlateMesh");
  scene->SetEntityMesh(plate_id, plate_mesh_id);

  Material plate_material;
  scene->SetEntityMaterial(plate_id, plate_material);

  // Cups
  int cups_id = scene->CreateEntity();

  Mesh cups_mesh;
  cups_mesh.LoadObjFile(FindAssetsFile("mesh/living/cups.obj"));
  auto cups_mesh_id =
      asset_manager->LoadMesh(cups_mesh, "CupsMesh");
  scene->SetEntityMesh(cups_id, cups_mesh_id);

  Material cups_material;
  scene->SetEntityMaterial(cups_id, cups_material);

  // Ceiling Light
  int ceilinglight_id = scene->CreateEntity();

  Mesh ceilinglight_mesh;
  ceilinglight_mesh.LoadObjFile(FindAssetsFile("mesh/living/ceilinglight.obj"));
  auto ceilinglight_mesh_id =
      asset_manager->LoadMesh(ceilinglight_mesh, "CeilingLightMesh");
  Material ceilinglight_material;
  ceilinglight_material.base_color = {0.3f, 0.3f, 0.3f};
  ceilinglight_material.emission = {1.0f, 1.0f, 1.0f};
  ceilinglight_material.emission_strength = 100.0f;
  scene->SetEntityMesh(ceilinglight_id, ceilinglight_mesh_id);
  scene->SetEntityMaterial(ceilinglight_id, ceilinglight_material);

  // Ceiling Light Edge
  int ceilinglightedge_id = scene->CreateEntity();

  Texture ceilinglightedge_texture;
  ceilinglightedge_texture.LoadFromFile(
      FindAssetsFile("texture/living/Wood09_diffuse_xtm.jpg"),
      LDRColorSpace::UNORM);
  auto ceilinglightedge_texture_id = asset_manager->LoadTexture(
      ceilinglightedge_texture, "CeilingLightEdgeTexture");
  scene->SetEntityAlbedoTexture(ceilinglightedge_id,
                                ceilinglightedge_texture_id);

  Mesh ceilinglightedge_mesh;
  ceilinglightedge_mesh.LoadObjFile(
      FindAssetsFile("mesh/living/ceilinglightedge.obj"));
  auto ceilinglightedge_mesh_id =
      asset_manager->LoadMesh(ceilinglightedge_mesh, "CeilingLightEdgeMesh");
  Material ceilinglightedge_material;
  scene->SetEntityMesh(ceilinglightedge_id, ceilinglightedge_mesh_id);
  scene->SetEntityMaterial(ceilinglightedge_id, ceilinglightedge_material);

  // Tea Table
  int teatable_id = scene->CreateEntity();

  Mesh teatable_mesh;
  teatable_mesh.LoadObjFile(FindAssetsFile("mesh/living/teatable.obj"));
  auto teatable_mesh_id =
      asset_manager->LoadMesh(teatable_mesh, "TeaTableMesh");
  scene->SetEntityMesh(teatable_id, teatable_mesh_id);

  Material teatable_material;
  scene->SetEntityMaterial(teatable_id, teatable_material);

  // Sofa Left Lower Cushion
  int sofaleftlowercushion_id = scene->CreateEntity();

  Mesh sofaleftlowercushion_mesh;
  sofaleftlowercushion_mesh.LoadObjFile(FindAssetsFile("mesh/living/sofaleftlowercushion.obj"));
  auto sofaleftlowercushion_mesh_id =
      asset_manager->LoadMesh(sofaleftlowercushion_mesh, "SofaLeftLowerCushionMesh");
  scene->SetEntityMesh(sofaleftlowercushion_id, sofaleftlowercushion_mesh_id);

  Material sofaleftlowercushion_material;
  scene->SetEntityMaterial(sofaleftlowercushion_id, sofaleftlowercushion_material);

  // Sofa Left Upper Cushion
  int sofaleftuppercushion_id = scene->CreateEntity();

  Mesh sofaleftuppercushion_mesh;
  sofaleftuppercushion_mesh.LoadObjFile(FindAssetsFile("mesh/living/sofaleftuppercushion.obj"));
  auto sofaleftuppercushion_mesh_id =
      asset_manager->LoadMesh(sofaleftuppercushion_mesh, "SofaLeftUpperCushionMesh");
  scene->SetEntityMesh(sofaleftuppercushion_id, sofaleftuppercushion_mesh_id);

  Material sofaleftuppercushion_material;
  scene->SetEntityMaterial(sofaleftuppercushion_id, sofaleftuppercushion_material);

  // Sofa Right Lower Cushion
  int sofarightlowercushion_id = scene->CreateEntity();

  Mesh sofarightlowercushion_mesh;
  sofarightlowercushion_mesh.LoadObjFile(FindAssetsFile("mesh/living/sofarightlowercushion.obj"));
  auto sofarightlowercushion_mesh_id =
      asset_manager->LoadMesh(sofarightlowercushion_mesh, "SofaRightLowerCushionMesh");
  scene->SetEntityMesh(sofarightlowercushion_id, sofarightlowercushion_mesh_id);

  Material sofarightlowercushion_material;
  scene->SetEntityMaterial(sofarightlowercushion_id, sofarightlowercushion_material);

  // Sofa Right Upper Cushion
  int sofarightuppercushion_id = scene->CreateEntity();

  Mesh sofarightuppercushion_mesh;
  sofarightuppercushion_mesh.LoadObjFile(FindAssetsFile("mesh/living/sofarightuppercushion.obj"));
  auto sofarightuppercushion_mesh_id =
      asset_manager->LoadMesh(sofarightuppercushion_mesh, "SofaRightUpperCushionMesh");
  scene->SetEntityMesh(sofarightuppercushion_id, sofarightuppercushion_mesh_id);

  Material sofarightuppercushion_material;
  scene->SetEntityMaterial(sofarightuppercushion_id, sofarightuppercushion_material);

  // Sofa Left Arm
  int sofaleftarm_id = scene->CreateEntity();

  Mesh sofaleftarm_mesh;
  sofaleftarm_mesh.LoadObjFile(FindAssetsFile("mesh/living/sofaleftarm.obj"));
  auto sofaleftarm_mesh_id =
      asset_manager->LoadMesh(sofaleftarm_mesh, "SofaLeftArmMesh");
  scene->SetEntityMesh(sofaleftarm_id, sofaleftarm_mesh_id);

  Material sofaleftarm_material;
  scene->SetEntityMaterial(sofaleftarm_id, sofaleftarm_material);

  // Sofa Right Arm
  int sofarightarm_id = scene->CreateEntity();

  Mesh sofarightarm_mesh;
  sofarightarm_mesh.LoadObjFile(FindAssetsFile("mesh/living/sofarightarm.obj"));
  auto sofarightarm_mesh_id =
      asset_manager->LoadMesh(sofarightarm_mesh, "SofaRightArmMesh");
  scene->SetEntityMesh(sofarightarm_id, sofarightarm_mesh_id);

  Material sofarightarm_material;
  scene->SetEntityMaterial(sofarightarm_id, sofarightarm_material);

  // Sofa Left Side Back Cushions
  int sofaleftsidebackcushions_id = scene->CreateEntity();

  Mesh sofaleftsidebackcushions_mesh;
  sofaleftsidebackcushions_mesh.LoadObjFile(FindAssetsFile("mesh/living/sofaleftsidebackcushions.obj"));
  auto sofaleftsidebackcushions_mesh_id =
      asset_manager->LoadMesh(sofaleftsidebackcushions_mesh, "SofaLeftSideBackCushionsMesh");
  scene->SetEntityMesh(sofaleftsidebackcushions_id, sofaleftsidebackcushions_mesh_id);

  Material sofaleftsidebackcushions_material;
  scene->SetEntityMaterial(sofaleftsidebackcushions_id, sofaleftsidebackcushions_material);

  // Sofa Left Middle Back Cushion
  int sofaleftmiddlebackcushion_id = scene->CreateEntity();

  Mesh sofaleftmiddlebackcushion_mesh;
  sofaleftmiddlebackcushion_mesh.LoadObjFile(FindAssetsFile("mesh/living/sofaleftmiddlebackcushion.obj"));
  auto sofaleftmiddlebackcushion_mesh_id =
      asset_manager->LoadMesh(sofaleftmiddlebackcushion_mesh, "SofaLeftMiddleBackCushionMesh");
  scene->SetEntityMesh(sofaleftmiddlebackcushion_id, sofaleftmiddlebackcushion_mesh_id);

  Material sofaleftmiddlebackcushion_material;
  scene->SetEntityMaterial(sofaleftmiddlebackcushion_id, sofaleftmiddlebackcushion_material);

  // Sofa Right Side Back Cushions
  int sofarightsidebackcushions_id = scene->CreateEntity();

  Mesh sofarightsidebackcushions_mesh;
  sofarightsidebackcushions_mesh.LoadObjFile(FindAssetsFile("mesh/living/sofarightsidebackcushions.obj"));
  auto sofarightsidebackcushions_mesh_id =
      asset_manager->LoadMesh(sofarightsidebackcushions_mesh, "SofaRightSideBackCushionsMesh");
  scene->SetEntityMesh(sofarightsidebackcushions_id, sofarightsidebackcushions_mesh_id);

  Material sofarightsidebackcushions_material;
  scene->SetEntityMaterial(sofarightsidebackcushions_id, sofarightsidebackcushions_material);

  // Sofa Right Middle Back Cushion
  int sofarightmiddlebackcushion_id = scene->CreateEntity();

  Mesh sofarightmiddlebackcushion_mesh;
  sofarightmiddlebackcushion_mesh.LoadObjFile(FindAssetsFile("mesh/living/sofarightmiddlebackcushion.obj"));
  auto sofarightmiddlebackcushion_mesh_id =
      asset_manager->LoadMesh(sofarightmiddlebackcushion_mesh, "SofaRightMiddleBackCushionMesh");
  scene->SetEntityMesh(sofarightmiddlebackcushion_id, sofarightmiddlebackcushion_mesh_id);

  Material sofarightmiddlebackcushion_material;
  scene->SetEntityMaterial(sofarightmiddlebackcushion_id, sofarightmiddlebackcushion_material);

  // Sofa Left Back
  int sofaleftback_id = scene->CreateEntity();

  Mesh sofaleftback_mesh;
  sofaleftback_mesh.LoadObjFile(FindAssetsFile("mesh/living/sofaleftback.obj"));
  auto sofaleftback_mesh_id =
      asset_manager->LoadMesh(sofaleftback_mesh, "SofaLeftBackMesh");
  scene->SetEntityMesh(sofaleftback_id, sofaleftback_mesh_id);

  Material sofaleftback_material;
  scene->SetEntityMaterial(sofaleftback_id, sofaleftback_material);

  // Sofa Right Back
  int sofarightback_id = scene->CreateEntity();

  Mesh sofarightback_mesh;
  sofarightback_mesh.LoadObjFile(FindAssetsFile("mesh/living/sofarightback.obj"));
  auto sofarightback_mesh_id =
      asset_manager->LoadMesh(sofarightback_mesh, "SofaRightBackMesh");
  scene->SetEntityMesh(sofarightback_id, sofarightback_mesh_id);

  Material sofarightback_material;
  scene->SetEntityMaterial(sofarightback_id, sofarightback_material);

  // Sofa Left Pillows
  int sofaleftpillows_id = scene->CreateEntity();

  Mesh sofaleftpillows_mesh;
  sofaleftpillows_mesh.LoadObjFile(FindAssetsFile("mesh/living/sofaleftpillows.obj"));
  auto sofaleftpillows_mesh_id =
      asset_manager->LoadMesh(sofaleftpillows_mesh, "SofaLeftPillowsMesh");
  scene->SetEntityMesh(sofaleftpillows_id, sofaleftpillows_mesh_id);

  Material sofaleftpillows_material;
  scene->SetEntityMaterial(sofaleftpillows_id, sofaleftpillows_material);

  // Sofa Right Pillows
  int sofarightpillows_id = scene->CreateEntity();

  Mesh sofarightpillows_mesh;
  sofarightpillows_mesh.LoadObjFile(FindAssetsFile("mesh/living/sofarightpillows.obj"));
  auto sofarightpillows_mesh_id =
      asset_manager->LoadMesh(sofarightpillows_mesh, "SofaRightPillowsMesh");
  scene->SetEntityMesh(sofarightpillows_id, sofarightpillows_mesh_id);

  Material sofarightpillows_material;
  scene->SetEntityMaterial(sofarightpillows_id, sofarightpillows_material);

  // Sofa Middle Pillow
  int sofamiddlepillow_id = scene->CreateEntity();

  Mesh sofamiddlepillow_mesh;
  sofamiddlepillow_mesh.LoadObjFile(FindAssetsFile("mesh/living/sofamiddlepillow.obj"));
  auto sofamiddlepillow_mesh_id =
      asset_manager->LoadMesh(sofamiddlepillow_mesh, "SofaMiddlePillowMesh");
  scene->SetEntityMesh(sofamiddlepillow_id, sofamiddlepillow_mesh_id);

  Material sofamiddlepillow_material;
  scene->SetEntityMaterial(sofamiddlepillow_id, sofamiddlepillow_material);

  // Bottom Pillar
  int bottompillar_id = scene->CreateEntity();

  Mesh bottompillar_mesh;
  bottompillar_mesh.LoadObjFile(FindAssetsFile("mesh/living/bottompillar.obj"));
  auto bottompillar_mesh_id =
      asset_manager->LoadMesh(bottompillar_mesh, "BottomPillarMesh");
  scene->SetEntityMesh(bottompillar_id, bottompillar_mesh_id);

  Material bottompillar_material;
  scene->SetEntityMaterial(bottompillar_id, bottompillar_material);

  // Television Edge
  int televisionedge_id = scene->CreateEntity();

  Mesh televisionedge_mesh;
  televisionedge_mesh.LoadObjFile(FindAssetsFile("mesh/living/televisionedge.obj"));
  auto televisionedge_mesh_id =
      asset_manager->LoadMesh(televisionedge_mesh, "TelevisionEdgeMesh");
  scene->SetEntityMesh(televisionedge_id, televisionedge_mesh_id);

  Material televisionedge_material;
  scene->SetEntityMaterial(televisionedge_id, televisionedge_material);

  // Television Center
  int televisioncenter_id = scene->CreateEntity();

  Mesh televisioncenter_mesh;
  televisioncenter_mesh.LoadObjFile(FindAssetsFile("mesh/living/televisioncenter.obj"));
  auto televisioncenter_mesh_id =
      asset_manager->LoadMesh(televisioncenter_mesh, "TelevisionCenterMesh");
  scene->SetEntityMesh(televisioncenter_id, televisioncenter_mesh_id);

  Material televisioncenter_material;
  scene->SetEntityMaterial(televisioncenter_id, televisioncenter_material);

  // Big Stereo 1 (shell without front face and top face)
  int bigstereo1_id = scene->CreateEntity();

  Mesh bigstereo1_mesh;
  bigstereo1_mesh.LoadObjFile(FindAssetsFile("mesh/living/bigstereo1.obj"));
  auto bigstereo1_mesh_id =
      asset_manager->LoadMesh(bigstereo1_mesh, "BigStereo1Mesh");
  scene->SetEntityMesh(bigstereo1_id, bigstereo1_mesh_id);

  Material bigstereo1_material;
  bigstereo1_material.base_color = {1.0f, 0.0f, 0.0f};
  scene->SetEntityMaterial(bigstereo1_id, bigstereo1_material);

  // Big Stereo 2 (front face and top face without the middle band)
  int bigstereo2_id = scene->CreateEntity();

  Mesh bigstereo2_mesh;
  bigstereo2_mesh.LoadObjFile(FindAssetsFile("mesh/living/bigstereo2.obj"));
  auto bigstereo2_mesh_id =
      asset_manager->LoadMesh(bigstereo2_mesh, "BigStereo2Mesh");
  scene->SetEntityMesh(bigstereo2_id, bigstereo2_mesh_id);

  Texture bigstereo2_texture;
  bigstereo2_texture.LoadFromFile(
      FindAssetsFile("texture/living/stereo.jpg"), LDRColorSpace::UNORM);
  auto bigstereo2_texture_id = asset_manager->LoadTexture(
      bigstereo2_texture, "BigStereo2Texture");
  scene->SetEntityAlbedoDetailTexture(bigstereo2_id, bigstereo2_texture_id);

  Material bigstereo2_material;
  bigstereo2_material.base_color = {0.0f, 1.0f, 0.0f};
  scene->SetEntityMaterial(bigstereo2_id, bigstereo2_material);

  // Big Stereo 3 (middle band without the sounding circle)
  int bigstereo3_id = scene->CreateEntity();

  Mesh bigstereo3_mesh;
  bigstereo3_mesh.LoadObjFile(FindAssetsFile("mesh/living/bigstereo3.obj"));
  auto bigstereo3_mesh_id =
      asset_manager->LoadMesh(bigstereo3_mesh, "BigStereo3Mesh");
  scene->SetEntityMesh(bigstereo3_id, bigstereo3_mesh_id);

  Material bigstereo3_material;
  bigstereo3_material.base_color = {0.0f, 0.0f, 1.0f};
  scene->SetEntityMaterial(bigstereo3_id, bigstereo3_material);

  // Big Stereo 4 (the sounding circle)
  int bigstereo4_id = scene->CreateEntity();

  Mesh bigstereo4_mesh;
  bigstereo4_mesh.LoadObjFile(FindAssetsFile("mesh/living/bigstereo4.obj"));
  auto bigstereo4_mesh_id =
      asset_manager->LoadMesh(bigstereo4_mesh, "BigStereo4Mesh");
  scene->SetEntityMesh(bigstereo4_id, bigstereo4_mesh_id);

  Material bigstereo4_material;
  scene->SetEntityMaterial(bigstereo4_id, bigstereo4_material);

  // Small Stereo Foot
  int smallstereofoot_id = scene->CreateEntity();

  Mesh smallstereofoot_mesh;
  smallstereofoot_mesh.LoadObjFile(FindAssetsFile("mesh/living/smallstereofoot.obj"));
  auto smallstereofoot_mesh_id =
      asset_manager->LoadMesh(smallstereofoot_mesh, "SmallStereoFootMesh");
  scene->SetEntityMesh(smallstereofoot_id, smallstereofoot_mesh_id);

  Material smallstereofoot_material;
  scene->SetEntityMaterial(smallstereofoot_id, smallstereofoot_material);

  // Small Stereo Head 1 (shell without front face)
  int smallstereohead1_id = scene->CreateEntity();

  Mesh smallstereohead1_mesh;
  smallstereohead1_mesh.LoadObjFile(FindAssetsFile("mesh/living/smallstereohead1.obj"));
  auto smallstereohead1_mesh_id =
      asset_manager->LoadMesh(smallstereohead1_mesh, "SmallStereoHead1Mesh");
  scene->SetEntityMesh(smallstereohead1_id, smallstereohead1_mesh_id);

  Material smallstereohead1_material;
  smallstereohead1_material.base_color = {1.0f, 0.0f, 0.0f};
  scene->SetEntityMaterial(smallstereohead1_id, smallstereohead1_material);

  // Small Stereo Head 2 (front face)
  int smallstereohead2_id = scene->CreateEntity();

  Mesh smallstereohead2_mesh;
  smallstereohead2_mesh.LoadObjFile(FindAssetsFile("mesh/living/smallstereohead2.obj"));
  auto smallstereohead2_mesh_id =
      asset_manager->LoadMesh(smallstereohead2_mesh, "SmallStereoHead2Mesh");
  scene->SetEntityMesh(smallstereohead2_id, smallstereohead2_mesh_id);

  Texture smallstereohead2_texture;
  smallstereohead2_texture.LoadFromFile(
      FindAssetsFile("texture/living/stereo.jpg"), LDRColorSpace::UNORM);
  auto smallstereohead2_texture_id = asset_manager->LoadTexture(
      smallstereohead2_texture, "SmallStereoHead2Texture");
  scene->SetEntityAlbedoDetailTexture(smallstereohead2_id, smallstereohead2_texture_id);

  Material smallstereohead2_material;
  smallstereohead2_material.base_color = {0.0f, 1.0f, 0.0f};
  scene->SetEntityMaterial(smallstereohead2_id, smallstereohead2_material);

  // Left Upper Box
  int leftupperbox_id = scene->CreateEntity();

  Mesh leftupperbox_mesh;
  leftupperbox_mesh.LoadObjFile(FindAssetsFile("mesh/living/leftupperbox.obj"));
  auto leftupperbox_mesh_id =
      asset_manager->LoadMesh(leftupperbox_mesh, "LeftUpperBoxMesh");
  scene->SetEntityMesh(leftupperbox_id, leftupperbox_mesh_id);

  Material leftupperbox_material;
  scene->SetEntityMaterial(leftupperbox_id, leftupperbox_material);

  // Left Middle Box
  int leftmiddlebox_id = scene->CreateEntity();

  Mesh leftmiddlebox_mesh;
  leftmiddlebox_mesh.LoadObjFile(FindAssetsFile("mesh/living/leftmiddlebox.obj"));
  auto leftmiddlebox_mesh_id =
      asset_manager->LoadMesh(leftmiddlebox_mesh, "LeftMiddleBoxMesh");
  scene->SetEntityMesh(leftmiddlebox_id, leftmiddlebox_mesh_id);

  Material leftmiddlebox_material;
  scene->SetEntityMaterial(leftmiddlebox_id, leftmiddlebox_material);

  // Left Lower Box
  int leftlowerbox_id = scene->CreateEntity();

  Mesh leftlowerbox_mesh;
  leftlowerbox_mesh.LoadObjFile(FindAssetsFile("mesh/living/leftlowerbox.obj"));
  auto leftlowerbox_mesh_id =
      asset_manager->LoadMesh(leftlowerbox_mesh, "LeftLowerBoxMesh");
  scene->SetEntityMesh(leftlowerbox_id, leftlowerbox_mesh_id);

  Material leftlowerbox_material;
  scene->SetEntityMaterial(leftlowerbox_id, leftlowerbox_material);

  // Right Box
  int rightbox_id = scene->CreateEntity();

  Mesh rightbox_mesh;
  rightbox_mesh.LoadObjFile(FindAssetsFile("mesh/living/rightbox.obj"));
  auto rightbox_mesh_id =
      asset_manager->LoadMesh(rightbox_mesh, "RightBoxMesh");
  scene->SetEntityMesh(rightbox_id, rightbox_mesh_id);

  Material rightbox_material;
  scene->SetEntityMaterial(rightbox_id, rightbox_material);

  // Box Button
  int boxbutton_id = scene->CreateEntity();

  Mesh boxbutton_mesh;
  boxbutton_mesh.LoadObjFile(FindAssetsFile("mesh/living/boxbutton.obj"));
  auto boxbutton_mesh_id =
      asset_manager->LoadMesh(boxbutton_mesh, "BoxButtonMesh");
  scene->SetEntityMesh(boxbutton_id, boxbutton_mesh_id);

  Material boxbutton_material;
  scene->SetEntityMaterial(boxbutton_id, boxbutton_material);

  // Curtain
  int curtain_id = scene->CreateEntity();

  Mesh curtain_mesh;
  curtain_mesh.LoadObjFile(FindAssetsFile("mesh/living/curtain.obj"));
  auto curtain_mesh_id =
      asset_manager->LoadMesh(curtain_mesh, "CurtainMesh");
  scene->SetEntityMesh(curtain_id, curtain_mesh_id);

  Material curtain_material;
  scene->SetEntityMaterial(curtain_id, curtain_material);

  // Right Lights
  int rightlights_id = scene->CreateEntity();

  Mesh rightlights_mesh;
  rightlights_mesh.LoadObjFile(FindAssetsFile("mesh/living/rightlights.obj"));
  auto rightlights_mesh_id =
      asset_manager->LoadMesh(rightlights_mesh, "RightLightsMesh");
  scene->SetEntityMesh(rightlights_id, rightlights_mesh_id);

  Material rightlights_material;
  scene->SetEntityMaterial(rightlights_id, rightlights_material);

  // Short Cupboard
  int shortcupboard_id = scene->CreateEntity();

  Mesh shortcupboard_mesh;
  shortcupboard_mesh.LoadObjFile(FindAssetsFile("mesh/living/shortcupboard.obj"));
  auto shortcupboard_mesh_id =
      asset_manager->LoadMesh(shortcupboard_mesh, "ShortCupboardMesh");
  scene->SetEntityMesh(shortcupboard_id, shortcupboard_mesh_id);

  Material shortcupboard_material;
  scene->SetEntityMaterial(shortcupboard_id, shortcupboard_material);

  // Long Cupboard
  int longcupboard_id = scene->CreateEntity();

  Mesh longcupboard_mesh;
  longcupboard_mesh.LoadObjFile(FindAssetsFile("mesh/living/longcupboard.obj"));
  auto longcupboard_mesh_id =
      asset_manager->LoadMesh(longcupboard_mesh, "LongCupboardMesh");
  scene->SetEntityMesh(longcupboard_id, longcupboard_mesh_id);

  Material longcupboard_material;
  scene->SetEntityMaterial(longcupboard_id, longcupboard_material);

  // Lamp 1 (lamp base)
  int lamp1_id = scene->CreateEntity();

  Mesh lamp1_mesh;
  lamp1_mesh.LoadObjFile(FindAssetsFile("mesh/living/lamp1.obj"));
  auto lamp1_mesh_id =
      asset_manager->LoadMesh(lamp1_mesh, "Lamp1Mesh");
  scene->SetEntityMesh(lamp1_id, lamp1_mesh_id);

  Material lamp1_material;
  lamp1_material.base_color = {1.0f, 0.0f, 0.0f};
  scene->SetEntityMaterial(lamp1_id, lamp1_material);

  // Lamp 2 (lamp shade)
  int lamp2_id = scene->CreateEntity();

  Mesh lamp2_mesh;
  lamp2_mesh.LoadObjFile(FindAssetsFile("mesh/living/lamp2.obj"));
  auto lamp2_mesh_id =
      asset_manager->LoadMesh(lamp2_mesh, "Lamp2Mesh");
  scene->SetEntityMesh(lamp2_id, lamp2_mesh_id);

  Material lamp2_material;
  lamp2_material.base_color = {0.0f, 1.0f, 0.0f};
  scene->SetEntityMaterial(lamp2_id, lamp2_material);

  // Sculpture
  int sculpture_id = scene->CreateEntity();

  Mesh sculpture_mesh;
  sculpture_mesh.LoadObjFile(FindAssetsFile("mesh/living/sculpture.obj"));
  auto sculpture_mesh_id =
      asset_manager->LoadMesh(sculpture_mesh, "SculptureMesh");
  scene->SetEntityMesh(sculpture_id, sculpture_mesh_id);

  Material sculpture_material;
  scene->SetEntityMaterial(sculpture_id, sculpture_material);

  // Sculpture Base
  int sculpturebase_id = scene->CreateEntity();

  Mesh sculpturebase_mesh;
  sculpturebase_mesh.LoadObjFile(FindAssetsFile("mesh/living/sculpturebase.obj"));
  auto sculpturebase_mesh_id =
      asset_manager->LoadMesh(sculpturebase_mesh, "SculptureBaseMesh");
  scene->SetEntityMesh(sculpturebase_id, sculpturebase_mesh_id);

  Material sculpturebase_material;
  scene->SetEntityMaterial(sculpturebase_id, sculpturebase_material);

  // Picture Edge
  int pictureedge_id = scene->CreateEntity();

  Mesh pictureedge_mesh;
  pictureedge_mesh.LoadObjFile(FindAssetsFile("mesh/living/pictureedge.obj"));
  auto pictureedge_mesh_id =
      asset_manager->LoadMesh(pictureedge_mesh, "PictureEdgeMesh");
  scene->SetEntityMesh(pictureedge_id, pictureedge_mesh_id);

  Material pictureedge_material;
  scene->SetEntityMaterial(pictureedge_id, pictureedge_material);

  // Picture Center
  int picturecenter_id = scene->CreateEntity();

  Mesh picturecenter_mesh;
  picturecenter_mesh.LoadObjFile(FindAssetsFile("mesh/living/picturecenter.obj"));
  auto picturecenter_mesh_id =
      asset_manager->LoadMesh(picturecenter_mesh, "PictureCenterMesh");
  scene->SetEntityMesh(picturecenter_id, picturecenter_mesh_id);

  Texture picturecenter_texture;
  picturecenter_texture.LoadFromFile(
      FindAssetsFile("texture/living/picture.jpg"), LDRColorSpace::UNORM);
  auto picturecenter_texture_id = asset_manager->LoadTexture(
      picturecenter_texture, "PictureCenterTexture");
  scene->SetEntityAlbedoTexture(picturecenter_id, picturecenter_texture_id);

  Material picturecenter_material;
  scene->SetEntityMaterial(picturecenter_id, picturecenter_material);

  // LongMa Spirit 1 (the edge)
  int LMspirit1_id = scene->CreateEntity();

  Mesh LMspirit1_mesh;
  LMspirit1_mesh.LoadObjFile(FindAssetsFile("mesh/living/LMspirit1.obj"));
  auto LMspirit1_mesh_id =
      asset_manager->LoadMesh(LMspirit1_mesh, "LongMaSpirit1Mesh");
  scene->SetEntityMesh(LMspirit1_id, LMspirit1_mesh_id);

  Material LMspirit1_material;
  LMspirit1_material.base_color = {1.0f, 0.0f, 0.0f};
  scene->SetEntityMaterial(LMspirit1_id, LMspirit1_material);

  // LongMa Spirit 2 (the inner part around the actual picture)
  int LMspirit2_id = scene->CreateEntity();

  Mesh LMspirit2_mesh;
  LMspirit2_mesh.LoadObjFile(FindAssetsFile("mesh/living/LMspirit2.obj"));
  auto LMspirit2_mesh_id =
      asset_manager->LoadMesh(LMspirit2_mesh, "LongMaSpirit2Mesh");
  scene->SetEntityMesh(LMspirit2_id, LMspirit2_mesh_id);

  Material LMspirit2_material;
  LMspirit2_material.base_color = {0.0f, 1.0f, 0.0f};
  scene->SetEntityMaterial(LMspirit2_id, LMspirit2_material);

  // LongMa Spirit 3 (the main picture)
  int LMspirit3_id = scene->CreateEntity();

  Mesh LMspirit3_mesh;
  LMspirit3_mesh.LoadObjFile(FindAssetsFile("mesh/living/LMspirit3.obj"));
  auto LMspirit3_mesh_id =
      asset_manager->LoadMesh(LMspirit3_mesh, "LongMaSpirit3Mesh");
  scene->SetEntityMesh(LMspirit3_id, LMspirit3_mesh_id);

  Texture LMspirit3_texture;
  LMspirit3_texture.LoadFromFile(
      FindAssetsFile("texture/living/LMspirit.jpg"), LDRColorSpace::UNORM);
  auto LMspirit3_texture_id = asset_manager->LoadTexture(
      LMspirit3_texture, "LongMaSpirit3Texture");
  scene->SetEntityAlbedoTexture(LMspirit3_id, LMspirit3_texture_id);

  Material LMspirit3_material;
  scene->SetEntityMaterial(LMspirit3_id, LMspirit3_material);

  // Inner Sculpture
  int innersculpture_id = scene->CreateEntity();

  Mesh innersculpture_mesh;
  innersculpture_mesh.LoadObjFile(FindAssetsFile("mesh/living/innersculpture.obj"));
  auto innersculpture_mesh_id =
      asset_manager->LoadMesh(innersculpture_mesh, "InnerSculptureMesh");
  scene->SetEntityMesh(innersculpture_id, innersculpture_mesh_id);

  Material innersculpture_material;
  scene->SetEntityMaterial(innersculpture_id, innersculpture_material);

  // Left Glass 1 (base, framework, and the back face)
  int leftglass1_id = scene->CreateEntity();

  Mesh leftglass1_mesh;
  leftglass1_mesh.LoadObjFile(FindAssetsFile("mesh/living/leftglass1.obj"));
  auto leftglass1_mesh_id =
      asset_manager->LoadMesh(leftglass1_mesh, "LeftGlass1Mesh");
  scene->SetEntityMesh(leftglass1_id, leftglass1_mesh_id);

  Material leftglass1_material;
  leftglass1_material.base_color = {1.0f, 0.0f, 0.0f};
  scene->SetEntityMaterial(leftglass1_id, leftglass1_material);

  // Left Glass 2 (the other faces)
  int leftglass2_id = scene->CreateEntity();

  Mesh leftglass2_mesh;
  leftglass2_mesh.LoadObjFile(FindAssetsFile("mesh/living/leftglass2.obj"));
  auto leftglass2_mesh_id =
      asset_manager->LoadMesh(leftglass2_mesh, "LeftGlass2Mesh");
  scene->SetEntityMesh(leftglass2_id, leftglass2_mesh_id);

  Material leftglass2_material;
  leftglass2_material.base_color = {0.0f, 1.0f, 0.0f};
  scene->SetEntityMaterial(leftglass2_id, leftglass2_material);

  // Right Glass 1 (the inner content)
  int rightglass1_id = scene->CreateEntity();

  Mesh rightglass1_mesh;
  rightglass1_mesh.LoadObjFile(FindAssetsFile("mesh/living/rightglass1.obj"));
  auto rightglass1_mesh_id =
      asset_manager->LoadMesh(rightglass1_mesh, "RightGlass1Mesh");
  scene->SetEntityMesh(rightglass1_id, rightglass1_mesh_id);

  Material rightglass1_material;
  rightglass1_material.base_color = {1.0f, 0.0f, 0.0f};
  scene->SetEntityMaterial(rightglass1_id, rightglass1_material);

  // Right Glass 2 (the base and the framework)
  int rightglass2_id = scene->CreateEntity();

  Mesh rightglass2_mesh;
  rightglass2_mesh.LoadObjFile(FindAssetsFile("mesh/living/rightglass2.obj"));
  auto rightglass2_mesh_id =
      asset_manager->LoadMesh(rightglass2_mesh, "RightGlass2Mesh");
  scene->SetEntityMesh(rightglass2_id, rightglass2_mesh_id);

  Material rightglass2_material;
  rightglass2_material.base_color = {0.0f, 1.0f, 0.0f};
  scene->SetEntityMaterial(rightglass2_id, rightglass2_material);

  // Right Glass 3 (other faces of the glass)
  int rightglass3_id = scene->CreateEntity();

  Mesh rightglass3_mesh;
  rightglass3_mesh.LoadObjFile(FindAssetsFile("mesh/living/rightglass3.obj"));
  auto rightglass3_mesh_id =
      asset_manager->LoadMesh(rightglass3_mesh, "RightGlass3Mesh");
  scene->SetEntityMesh(rightglass3_id, rightglass3_mesh_id);

  Material rightglass3_material;
  rightglass3_material.base_color = {0.0f, 0.0f, 1.0f};
  scene->SetEntityMaterial(rightglass3_id, rightglass3_material);

  scene->Camera()->SetFar(500.0f);
  scene->Camera()->SetNear(0.05f);
  scene->Camera()->SetPosition({0.0f, 0.1f, 1.2f});
  scene->Camera()->SetCameraSpeed(2.0f);
}

}  // namespace sparkium