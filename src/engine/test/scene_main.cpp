#include "scene_main.h"

#include <imgui.h>
#include <imgui_file/ImGuiFileDialog.h>

#include <entt/entt.hpp>

#include "components.h"
#include "engine/application/event.h"
#include "engine/application/key_codes.h"
#include "engine/ecs/component/renderer_components.h"
#include "engine/ecs/component/transform.h"
#include "engine/ecs/system/isystem.h"
#include "engine/ecs/system/window_system.h"
#include "engine/pch.h"
#include "engine/renderer/material.h"
#include "engine/resource/material_manager.h"
#include "engine/resource/mesh_manager.h"
#include "engine/resource/paths.h"
#include "engine/resource/shader_manager.h"
#include "engine/timestep.h"
#include "systems.h"

void SceneMain::Init() {
  std::vector<engine::ecs::ISystem*> systems = {&camera_system_};
  InitSystems(systems);

  std::string model_string =
      // "/home/tony/dep/models/glTF-Sample-Assets/Models/Sponza/glTF/Sponza.gltf";
      "/home/tony/dep/models/glTF-Sample-Assets/Models/WaterBottle/glTF/WaterBottle.gltf";
  auto gear_mesh_materials = mesh_manager_->LoadModel(model_string);
  // mesh_manager_->LoadModel(GET_PATH("resources/models/Gear1/Gear1.gltf"));
  // mesh_manager_->LoadModel(
  //     "/home/tony/dep/models/glTF-Sample-Assets/Models/Sponza/glTF/Sponza.gltf");
  // mesh_manager_->LoadModel(
  //     "/home/tony/personal/opengl_renderer/resources/models/sponza/sponza.obj");
  glm::vec3 iter{0};
  auto scale = glm::vec3(.01);
  int c = 1;
  for (iter.z = -c; iter.z <= c; iter.z++) {
    for (iter.x = -c; iter.x <= c; iter.x++) {
      for (auto m : gear_mesh_materials) {
        engine::component::Transform t;
        t.SetScale(scale);
        auto ent = registry.create();
        t.SetTranslation({iter.x * 50, iter.y, iter.z * 50});
        registry.emplace<engine::component::MeshMaterial>(ent, m);
        registry.emplace<engine::component::Transform>(ent, t);
        registry.emplace<engine::component::ModelMatrix>(ent);
      }
    }
  }
  ecs::LoadCamera(registry, *window_system_);
}

// SceneMain::SceneMain() {
//   glm::vec3 iter{0, 0, 0};
//   engine::MeshID mesh_id = mesh_manager_->LoadShape(engine::ShapeType::Cube);
//   MaterialCreateInfo mat_create_info;
//   mat_create_info.base_color = {1., 0., 1.};
//   engine::MaterialID color_only_mat = material_manager_->AddMaterial(mat_create_info);
//   component::Transform t;
//   float r = 0;
//
//   std::vector<component::Material> data;
//   int num_mats = 100;
//   data.reserve(num_mats);
//   MaterialCreateInfo d;
//   for (int i = 0; i < num_mats; i++) {
//     d.base_color = {rand() % num_mats / static_cast<float>(num_mats), rand() % num_mats /
//     num_mats,
//                     rand() % num_mats / num_mats};
//     engine::MaterialID itermat = material_manager_->AddMaterial(d);
//     data.push_back({itermat});
//   }
//
//   for (iter.x = -40; iter.x <= 40; iter.x++) {
//     for (iter.y = -40; iter.y <= 40; iter.y++) {
//       for (iter.z = 0; iter.z <= 0; iter.z++) {
//         entt::entity tri{};
//         if (static_cast<int>(iter.z + iter.x) % 2 == 0) {
//           tri = MakeDynamicEntity();
//         } else {
//           tri = MakeStaticEntity();
//         }
//         t.SetTranslation({iter.x * 2, iter.y * 2, iter.z * 2});
//         registry.emplace<component::Transform>(tri, t);
//         registry.emplace<component::ModelMatrix>(tri);
//         registry.emplace<component::Mesh>(tri, mesh_id);
//         registry.emplace<component::Material>(tri, data[rand() % num_mats]);
//       }
//     }
//   }
//
//   bool start_fps_focus = true;
//   auto player = registry.create();
//   registry.emplace<Player>(player).fps_focused = start_fps_focus;
//   WindowSystem::Get().SetCursorVisible(!start_fps_focus);
//
//   component::FPSCamera fps_cam;
//   fps_cam.position = {2, 1, 1};
//   registry.emplace<component::FPSCamera>(player, fps_cam);
// }

void SceneMain::OnEvent(const engine::Event& e) { camera_system_.OnEvent(e); }

void SceneMain::OnUpdate(engine::Timestep timestep) { camera_system_.OnUpdate(timestep); }

void drawGui(void (*func)(const std::string&, const std::string&)) {
  // open Dialog Simple
}

void DrawImGuiDropdown(const char* label, std::vector<std::string>& items, int& currentItemIndex) {
  if (ImGui::BeginCombo(label, items[currentItemIndex].data())) {
    for (int i = 0; i < items.size(); i++) {
      bool is_selected = (currentItemIndex == i);
      if (ImGui::Selectable(items[i].data(), is_selected)) currentItemIndex = i;

      if (is_selected)
        ImGui::SetItemDefaultFocus();  // Set the initial focus when opening the combo
    }
    ImGui::EndCombo();
  }
}

void SceneMain::OnImGuiRender() {
  ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoNavFocus);

  camera_system_.OnImGui();

  if (ImGui::Button("Open File Dialog")) {
    IGFD::FileDialogConfig config;
    config.path = ".";
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".gltf,.obj,.fbx",
                                            config);
  }
  auto& strings = ecs::GetModelPaths();
  static int i = 0;
  DrawImGuiDropdown("dropdown", strings, i);
  if (ImGui::Button("Load")) {
    auto v = registry.view<engine::component::Transform>();
    registry.destroy(v.begin(), v.end());
    engine::component::Transform t;
    auto mesh_materials = mesh_manager_->LoadModel(strings[i]);
    ecs::ModelViewerLoadModel(registry, t, mesh_materials);
  }
  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
    if (ImGuiFileDialog::Instance()->IsOk()) {  // action if OK
      std::string file_path_name = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string file_path = ImGuiFileDialog::Instance()->GetCurrentPath();
      auto v = registry.view<engine::component::Transform>();
      registry.destroy(v.begin(), v.end());
      engine::component::Transform t;
      auto mesh_materials = mesh_manager_->LoadModel(file_path_name);
      ecs::ModelViewerLoadModel(registry, t, mesh_materials);
    }
    ImGuiFileDialog::Instance()->Close();
  }

  ImGui::End();
}