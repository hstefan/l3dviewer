#pragma once

#include <glad/glad.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

namespace l3d {
namespace gl {
class Shader {
 public:
  inline Shader(GLenum type);
  inline ~Shader();
  inline void Source(std::string src);
  inline void Source(std::istream& src);
  inline void SourceFromFile(std::string path);
  inline bool Compile();
  inline operator GLuint() const;

 private:
  GLuint shader;
};

Shader::Shader(GLenum type) : shader(0) {
  assert(type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER ||
         type == GL_GEOMETRY_SHADER && "Unsupported shader type!");
  shader = glCreateShader(type);
  assert(shader != 0 && "Unable to create shader!");
}

Shader::~Shader() {
  assert(shader != 0 && "Attempt to destroy an invalid shader!");
  glDeleteShader(shader);
}

void Shader::Source(std::string src) {
  assert(shader != 0 && "Attempt to provide source for invalid shader!");
  const GLchar* glSrc = src.c_str();
  glShaderSource(shader, 1, &glSrc, nullptr);
}

void Shader::Source(std::istream& src) {
  const std::string contents{std::istreambuf_iterator<GLchar>(src),
                             std::istreambuf_iterator<GLchar>()};
  Source(contents);
}

void Shader::SourceFromFile(std::string path) {
  std::ifstream stream(path);
  assert(stream && "Unable to open file!");
  Source(stream);
}

bool Shader::Compile() {
  assert(shader != 0 && "Attempt to compile an invalid shader!");
  GLint status;
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  assert(status == GL_TRUE && "Unable to compile shader!");
  // TODO: capture compile log and display it with some logging system
  return status == GL_TRUE;
}

Shader::operator GLuint() const { return shader; }

}  // namespace gl
}  // namespace l3d