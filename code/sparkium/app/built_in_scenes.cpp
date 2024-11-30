#include "sparkium/app/built_in_scenes.h"

#include "sparkium/asset_manager/asset_manager.h"
#include "sparkium/renderer/renderer.h"

namespace sparkium {

std::vector<std::pair<std::string, std::function<void(Scene *scene)>>>
BuiltInSceneList() {
  return {
      {"Cornell Box", LoadCornellBox},
      {"Island Scene", LoadIslandScene},
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



  indices = {0, 1,  3,  1,  2,  3,  4,  5,  7,  5,  6,  7,  8,  9,  11,
             9, 10, 11, 12, 13, 15, 13, 14, 15, 16, 17, 19, 17, 18, 19, 20, 21, 23, 21, 22, 23};
  vertices.clear();
  vertices.push_back(make_vertex({130.0f, 165.0f, 65.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({82.0f, 165.0f, 225.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({240.0f, 165.0f, 272.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({290.0f, 165.0f, 114.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({290.0f, 0.0f, 114.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({290.0f, 165.0f, 114.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({240.0f, 165.0f, 272.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({240.0f, 0.0f, 272.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({130.0f, 0.0f, 65.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({130.0f, 165.0f, 65.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({290.0f, 165.0f, 114.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({290.0f, 0.0f, 114.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({82.0f, 0.0f, 225.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({82.0f, 165.0f, 225.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({130.0f, 165.0f, 65.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({130.0f, 0.0f, 65.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({240.0f, 0.0f, 272.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({240.0f, 165.0f, 272.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({82.0f, 165.0f, 225.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({82.0f, 0.0f, 225.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({130.0f, 0.0f, 65.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({82.0f, 0.0f, 225.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({240.0f, 0.0f, 272.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({290.0f, 0.0f, 114.0f}, {0.0f, 1.0f}));
  int short_box_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "ShortBoxMesh");
  Material short_box_material;
  short_box_material.base_color = {0.8f, 0.8f, 0.8f};
  int short_box_id = scene->CreateEntity();
  scene->SetEntityMesh(short_box_id, short_box_mesh_id);
  scene->SetEntityMaterial(short_box_id, short_box_material);


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



  indices = {0, 1,  3,  1,  2,  3,  4,  5,  7,  5,  6,  7,  8,  9,  11,
             9, 10, 11, 12, 13, 15, 13, 14, 15, 16, 17, 19, 17, 18, 19, 20, 21, 23, 21, 22, 23};
  vertices.clear();
  vertices.push_back(make_vertex({423.0f, 330.0f, 247.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({265.0f, 330.0f, 296.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({314.0f, 330.0f, 456.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({472.0f, 330.0f, 406.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({423.0f, 0.0f, 247.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({423.0f, 330.0f, 247.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({472.0f, 330.0f, 406.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({472.0f, 0.0f, 406.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({472.0f, 0.0f, 406.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({472.0f, 330.0f, 406.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({314.0f, 330.0f, 456.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({314.0f, 0.0f, 456.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({314.0f, 0.0f, 456.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({314.0f, 330.0f, 456.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({265.0f, 330.0f, 296.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({265.0f, 0.0f, 296.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({265.0f, 0.0f, 296.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({265.0f, 330.0f, 296.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({423.0f, 330.0f, 247.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({423.0f, 0.0f, 247.0f}, {0.0f, 1.0f}));
  vertices.push_back(make_vertex({423.0f, 0.0f, 247.0f}, {0.0f, 0.0f}));
  vertices.push_back(make_vertex({265.0f, 0.0f, 296.0f}, {1.0f, 0.0f}));
  vertices.push_back(make_vertex({314.0f, 0.0f, 456.0f}, {1.0f, 1.0f}));
  vertices.push_back(make_vertex({472.0f, 0.0f, 406.0f}, {0.0f, 1.0f}));

  int tall_box_mesh_id =
      asset_manager->LoadMesh(Mesh(vertices, indices), "TallBoxMesh");
  Material tall_box_material;
  tall_box_material.base_color = {0.8f, 0.8f, 0.8f};
  int tall_box_id = scene->CreateEntity();
  scene->SetEntityMesh(tall_box_id, tall_box_mesh_id);
  scene->SetEntityMaterial(tall_box_id, tall_box_material);



  Mesh test;
  test.LoadObjFile(FindAssetsFile("mesh/cube.obj"));
  test.ThisIsPointLight();
  int test_mesh_id = asset_manager->LoadMesh(test, "Cube");
  Material test_material;
  test_material.base_color = {0.0f, 0.0f, 0.8f};
  test_material.emission = {1.0f, 1.0f, 1.0f};
  test_material.emission_strength = 0.0f;
  test_material.type = MATERIAL_TYPE_POINTLIGHT;  // Point Light
  test_material.center = {30.0f, 518.8f, 529.2f};
  int test_id = scene->CreateEntity();
  scene->SetEntityMesh(test_id, test_mesh_id);
  scene->SetEntityMaterial(test_id, test_material);

  Texture milkdragon_texture;
  milkdragon_texture.LoadFromFile(
      FindAssetsFile("texture/IIIS.png"),
      LDRColorSpace::UNORM);
  auto milkdragon_texture_id =
      asset_manager->LoadTexture(milkdragon_texture, "IIISTexture");
  scene->SetEntityAlbedoTexture(back_wall_id, milkdragon_texture_id);
  //scene->SetEntityAlbedoDetailTexture(back_wall_id, milkdragon_texture_id);

  Texture thu_texture;
  thu_texture.LoadFromFile(FindAssetsFile("texture/THU.png"),
                                  LDRColorSpace::UNORM);
  auto thu_texture_id =
      asset_manager->LoadTexture(thu_texture, "thuTexture");
  scene->SetEntityAlbedoTexture(back_wall_id, thu_texture_id);
  //scene->SetEntityAlbedoDetailTexture(back_wall_id, thu_texture_id);

  Texture normal_texture;
  normal_texture.LoadFromFile(FindAssetsFile("texture/normal.png"),
                                  LDRColorSpace::UNORM);
  auto normal_texture_id =
      asset_manager->LoadTexture(normal_texture, "NormalTexture");
  scene->SetEntityNormalTexture(back_wall_id, normal_texture_id);

  scene->SetEnvmapSettings({0.0f, 0.0f, 0, 0});
  auto envmap = scene->GetEnvMap();

  Texture envmap_texture;
  envmap_texture.LoadFromFile(FindAssetsFile("texture/envmap_clouds_4k.hdr"),
                              LDRColorSpace::UNORM);
  auto envmap_id = asset_manager->LoadTexture(envmap_texture, "Envmap");
  envmap->SetEnvmapTexture(envmap_id);
  scene->SetEnvmapSettings({0.0f, 1.0f, uint32_t(envmap_id), 0});

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

}  // namespace sparkium
