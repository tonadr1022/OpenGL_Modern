#include "scene_main.h"

#include <engine/application/event.h>
#include <engine/application/key_codes.h>
#include <engine/ecs/component/camera.h>
#include <engine/ecs/component/core_components.h>
#include <engine/ecs/component/renderer_components.h>
#include <engine/ecs/system/window_system.h>
#include <engine/renderer/material.h>
#include <engine/resource/material_manager.h>
#include <engine/resource/mesh_manager.h>
#include <engine/resource/paths.h>
#include <engine/resource/shader_manager.h>
#include <imgui.h>

#include <entt/core/hashed_string.hpp>
#include <entt/entt.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "components.h"
#include "engine/core/e_assert.h"
#include "engine/ecs/component/transform.h"
#include "systems.h"

SceneMain::SceneMain() : Scene("main") {}

void SceneMain::OnEvent(const engine::Event& e) {}

void SceneMain::OnImGuiRender() {
  ImGui::Begin("Scene");
  auto player_entity = registry.view<Player>().front();
  if (registry.any_of<component::FPSCamera>(player_entity)) {
    auto& camera = registry.get<component::FPSCamera>(player_entity);
    ecs::fps_cam_sys::OnImGui(camera);
  }

  // auto materials = registry.group<component::Material>();
  // materials.each([](component::Material& material) {
  //   auto& mat = gfx::MaterialManager::GetMaterial(material.handle);
  //   ImGui::SliderFloat3(std::string("Diffuse###" + std::to_string(material.handle)).c_str(),
  //                       glm::value_ptr(mat.diffuse), 0.0f, 1.0f);
  // });
  ImGui::End();
}

void SceneMain::OnUpdate(engine::Timestep timestep) {
  entt::entity player_entity = registry.view<Player>().front();
  if (!registry.any_of<component::FPSCamera>(player_entity)) return;
  auto& player = registry.get<Player>(player_entity);
  auto& camera = registry.get<component::FPSCamera>(player_entity);
  if (player.fps_focused) ecs::fps_cam_sys::OnUpdate(camera, timestep);
  current_camera_matrices.view_matrix =
      glm::lookAt(camera.position, camera.position + camera.front, {0., 1., 0.});
  auto dims = engine_->window_system_->GetWindowDimensions();
  float aspect_ratio = static_cast<float>(dims.x) / static_cast<float>(dims.y);
  current_camera_matrices.projection_matrix =
      glm::perspective(glm::radians(camera.fov), aspect_ratio, camera.near_plane, camera.far_plane);
}

void SceneMain::Load() {
  glm::vec3 iter{0, 0, 0};
  engine::MeshID mesh_id = engine::MeshManager::LoadShape(engine::ShapeType::Cube);
  engine::MaterialData mat;
  mat.diffuse = {1., 0., 1.};
  engine::MaterialID color_only_mat = engine::MaterialManager::AddMaterial(mat);
  component::Transform t;
  float r = 0;

  engine::MaterialData m;
  m.diffuse = {1, 1, 0};
  auto ship = registry.create();
  auto captain = registry.create();
  registry.emplace<component::Children>(ship).children = {captain};
  registry.emplace<component::Parent>(captain).parent = ship;

  registry.emplace<component::Transform>(ship, t);
  registry.emplace<component::LocalTransform>(ship);
  registry.emplace<component::ModelMatrix>(ship);
  registry.emplace<component::Mesh>(ship, mesh_id);
  registry.emplace<component::Material>(ship, m);

  registry.emplace<component::Transform>(captain, t);
  registry.emplace<component::LocalTransform>(captain, t);
  registry.emplace<component::ModelMatrix>(captain);
  registry.emplace<component::Mesh>(captain, mesh_id);
  m.diffuse = {0, 1, 1};
  registry.emplace<component::Material>(captain, m);

  // for (iter.x = -40; iter.x <= 40; iter.x++) {
  //   engine::MaterialData m;
  //   m.diffuse = {r, 0., 1.0 - r};
  //   r += 0.01;
  //   engine::MaterialID itermat = engine::MaterialManager::AddMaterial(m);
  //   for (iter.y = -40; iter.y <= 40; iter.y++) {
  //     for (iter.z = -4; iter.z <= 4; iter.z++) {
  //       auto tri = registry.create();
  //       t.SetTranslation({iter.x * 2, iter.y * 2, iter.z * 2});
  //       registry.emplace<component::Transform>(tri, t);
  //       registry.emplace<component::ModelMatrix>(tri);
  //       registry.emplace<component::Mesh>(tri, mesh_id);
  //       registry.emplace<component::Material>(tri, itermat);
  //     }
  //   }
  // }

  auto player = registry.create();
  registry.emplace<Player>(player).fps_focused = true;
  engine_->window_system_->SetCursorVisible(true);

  component::FPSCamera fps_cam;
  fps_cam.position = {2, 1, 1};
  registry.emplace<component::FPSCamera>(player, fps_cam);
}
