#include "Window.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>

using l3d::gl::Window;
using l3d::gl::ContextProperties;

Window::Window(const char* title, int width, int height,
               ContextProperties contextProp)
    : Title(title),
      Width(width),
      Height(height),
      ContextProp(contextProp),
      Win(nullptr) {
  const int glfwRc = glfwInit();
  if (!glfwRc)
    printf("Unable to initialize glfw, status %d", glfwRc);

  // configures window hints for the desired context properties
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ContextProp.MajorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ContextProp.MinorVersion);

  const int profile = ContextProp.CoreProfile ? GLFW_OPENGL_CORE_PROFILE
                                              : GLFW_OPENGL_COMPAT_PROFILE;
  glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                 ContextProp.ForwardCompatible ? GL_TRUE : GL_FALSE);

  Win = glfwCreateWindow(Width, Height, Title, nullptr, nullptr);
  glfwMakeContextCurrent(Win);

  const int gladRc = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  if (!gladRc)
    printf("Unable to initialize glad, status %d", glfwRc);
}

Window::~Window() {
  if (Win != nullptr) {
    glfwSetWindowShouldClose(Win, GL_TRUE);
    PollEvents();
    glfwTerminate();
  }
}

void Window::Close() {
  glfwSetWindowShouldClose(Win, GL_TRUE);
  PollEvents();
}

void Window::PollEvents() {
  glfwPollEvents();
}

void Window::SwapBuffers() {
  glfwSwapBuffers(Win);
}

bool Window::IsKeyPressed(int key) const {
  return glfwGetKey(Win, key) == GLFW_PRESS;
}

bool Window::IsOpen() const {
  return glfwWindowShouldClose(Win) == GL_FALSE;
}
