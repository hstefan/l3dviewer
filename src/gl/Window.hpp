#pragma once

struct GLFWwindow;

namespace l3d {
namespace gl {

struct ContextProperties {
  int MajorVersion;
  int MinorVersion;
  bool CoreProfile;
  bool ForwardCompatible;
};

class Window {
public:
  Window(const char* title, int width, int height,
         ContextProperties contextProp);
  virtual ~Window();
  void Close();
  void PollEvents();
  void SwapBuffers();
  bool IsKeyPressed(int key) const;
  bool IsOpen() const;

  const char* Title;
  const int Width;
  const int Height;
  const ContextProperties ContextProp;
private:
  GLFWwindow* Win;
};

}  // namespace gfx
}  // namespace hst
