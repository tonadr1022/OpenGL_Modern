#pragma once

#include <cstdint>
struct GLFWwindow;

class Window {
 public:
  void Init();
  void SwapBuffers();
  void SetVsync(bool state);
  void Shutdown();
  [[nodiscard]] bool GetVsync() const { return is_vsync_; };
  [[nodiscard]] GLFWwindow* GetContext() const { return glfw_window_; }

 private:
  bool is_vsync_;
  GLFWwindow* glfw_window_;
  uint32_t framebuffer_width_;
  uint32_t framebuffer_height_;
};
