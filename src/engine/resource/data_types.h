#pragma once

#include "engine/pch.h"

namespace engine {

using MaterialID = uint32_t;
using MeshID = uint32_t;

namespace gfx {

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 tex_coords;
};

using Index = uint32_t;

}  // namespace gfx

}  // namespace engine
