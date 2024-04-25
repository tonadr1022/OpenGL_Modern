#pragma once

#include "../../pch.h"

namespace component {

struct Transform {
 public:
  [[nodiscard]] const auto& GetTranslation() const { return translation_; }
  [[nodiscard]] const auto& GetRotation() const { return rotation_; }
  [[nodiscard]] const auto& GetScale() const { return scale_; }
  void SetModel() { dirty_ = false; }
  void SetTranslation(const glm::vec3& vec);
  void SetRotation(const glm::mat4& mat);
  void SetRotation(const glm::quat& quat);
  void SetScale(const glm::vec3& vec);

  glm::mat4 CalculateModel();

  [[nodiscard]] bool IsDirty() const { return dirty_; }

 private:
  glm::quat rotation_{1, 0, 0, 0};
  glm::vec3 scale_{1};
  glm::vec3 translation_{0};
  bool dirty_{true};
};

}  // namespace component
