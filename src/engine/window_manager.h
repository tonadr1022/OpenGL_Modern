#pragma once

#include <cstdint>
#include <glm/fwd.hpp>

struct GLFWwindow;
namespace engine {

class Engine;
struct WindowManager {
  static WindowManager& Get();
  void Init();
  void SwapBuffers();
  void SetVsync(bool state);
  void Shutdown();
  void SetCursorVisibility(bool state);
  void CenterCursor();
  void SetUserPointer(void* ptr);
  bool ShouldClose();

  [[nodiscard]] glm::vec2 GetWindowDimensions() const;
  [[nodiscard]] bool GetVsync() const { return is_vsync_; };
  [[nodiscard]] GLFWwindow* GetContext() const { return glfw_window_; }
  [[nodiscard]] bool GetCursorVisibility() const;

 private:
  // static class only to be created once by engine
  friend class Engine;
  WindowManager();
  static WindowManager* instance_;
  bool is_vsync_;
  GLFWwindow* glfw_window_;
  uint32_t framebuffer_width_;
  uint32_t framebuffer_height_;
};
}  // namespace engine
