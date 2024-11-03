#pragma once
#include "glm/gtc/matrix_transform.hpp"
#include "sparkium/asset_manager/asset_manager.h"
#include "sparkium/scene/scene.h"

namespace sparkium {

std::vector<std::pair<std::string, std::function<void(Scene *scene)>>>
BuiltInSceneList();

void LoadCornellBox(Scene *scene);

void LoadIslandScene(Scene *scene);

}  // namespace sparkium
