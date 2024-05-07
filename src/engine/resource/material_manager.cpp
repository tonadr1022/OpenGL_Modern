#include "material_manager.h"

#include <memory>
#include <vector>

#include "engine/renderer/gl/texture_2d.h"
#include "engine/renderer/material.h"
#include "engine/renderer/renderer.h"
#include "engine/resource/data_types.h"

namespace engine {

MaterialID MaterialManager::AddMaterial(const MaterialCreateInfo& material_create_info) {
  gfx::Texture2DCreateParams create_params;
  create_params.generate_mipmaps = true;
  create_params.bindless = true;
  create_params.s_rgb = true;

  gfx::MaterialData material;

  if (material_create_info.albedo_path.has_value()) {
    create_params.path = material_create_info.albedo_path.value();
    auto it = texture_map_.find(material_create_info.albedo_path.value());
    if (it != texture_map_.end()) {
      material.albedo_texture = it->second.get();
    } else {
      auto tex = std::make_unique<gfx::Texture2D>(create_params);
      texture_map_.emplace(material_create_info.albedo_path.value(), std::move(tex));
      material.albedo_texture = tex.get();
    }
  }

  if (material_create_info.metalness_path.has_value()) {
    create_params.path = material_create_info.metalness_path.value();
    auto it = texture_map_.find(material_create_info.metalness_path.value());
    if (it != texture_map_.end()) {
      material.metalness_texture = it->second.get();
    } else {
      auto tex = std::make_unique<gfx::Texture2D>(create_params);
      texture_map_.emplace(material_create_info.metalness_path.value(), std::move(tex));
      material.metalness_texture = tex.get();
    }
  }

  if (material_create_info.roughness_path.has_value()) {
    create_params.path = material_create_info.roughness_path.value();
    auto it = texture_map_.find(material_create_info.roughness_path.value());
    if (it != texture_map_.end()) {
      material.roughness_texture = it->second.get();
    } else {
      auto tex = std::make_unique<gfx::Texture2D>(create_params);
      texture_map_.emplace(material_create_info.roughness_path.value(), std::move(tex));
      material.roughness_texture = tex.get();
    }
  }

  if (material_create_info.normal_path.has_value()) {
    create_params.path = material_create_info.normal_path.value();
    auto it = texture_map_.find(material_create_info.normal_path.value());
    if (it != texture_map_.end()) {
      material.normal_texture = it->second.get();
    } else {
      auto tex = std::make_unique<gfx::Texture2D>(create_params);
      texture_map_.emplace(material_create_info.normal_path.value(), std::move(tex));
      material.normal_texture = tex.get();
    }
  }
  if (material_create_info.ao_path.has_value()) {
    create_params.path = material_create_info.ao_path.value();
    auto it = texture_map_.find(material_create_info.ao_path.value());
    if (it != texture_map_.end()) {
      material.ao_texture = it->second.get();
    } else {
      auto tex = std::make_unique<gfx::Texture2D>(create_params);
      texture_map_.emplace(material_create_info.ao_path.value(), std::move(tex));
      material.ao_texture = tex.get();
    }
  }

  MaterialID id = renderer_.AddMaterial(material);
  material_map_.emplace(id, material);
  return id;
}

MaterialManager::MaterialManager(gfx::Renderer& renderer) : renderer_(renderer) {}
void MaterialManager::Init() {
  gfx::MaterialData default_material;
  default_material.base_color = {0, 1, 0};
  default_material_id_ = renderer_.AddMaterial(default_material);
}

gfx::MaterialData& MaterialManager::GetMaterial(MaterialID id) {
  auto it = material_map_.find(id);
  EASSERT(it != material_map_.end());
  return it->second;
}

void MaterialManager::ClearMaterials() {
  material_map_.clear();
  material_counter_ = 0;
  // TODO(tony): clear from renderer
}

std::vector<std::pair<MaterialID, gfx::MaterialData>> MaterialManager::GetAllMaterials() const {
  return {material_map_.begin(), material_map_.end()};
}

}  // namespace engine
