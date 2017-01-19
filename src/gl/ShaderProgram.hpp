#pragma once

#include <glad/glad.h>
#include <cassert>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include "Shader.hpp"
#include "VertexArray.hpp"

namespace l3d {
namespace gl {

class ShaderProgram {
 public:
  inline ShaderProgram();
  inline ~ShaderProgram();
  inline void Attach(const Shader& shader);
  inline void Attach(GLuint shader);
  inline void BindFragmentLocation(const char* attr, int color);
  inline bool Link();
  inline void Use();
  inline void VertexAttribPointerf(GLuint vao, const char* attr, int index,
                                   GLint size, bool normalized,
                                   GLsizei groupSize);
  inline void VertexAttribPointerf(const VertexArray& vao, const char* attr,
                                   int index, GLint size, bool normalized,
                                   GLsizei groupSize);
  template <class T>
  inline void SetUniform(const char* name, const T& value);
  inline void SetUniform(GLint loc, float value);
  inline void SetUniform(GLint loc, int value);
  inline void SetUniform(GLint loc, const glm::mat4& value);
  inline void SetUniform(GLint loc, const glm::vec3& value);
  inline GLint GetUniformLocation(const char* name);

 private:
  GLuint prog;
};

ShaderProgram::ShaderProgram() : prog(0) {
  prog = glCreateProgram();
  assert(prog != 0 && "Unable to create program!");
}

ShaderProgram::~ShaderProgram() {
  assert(prog != 0 && "Attempt to destroy invalid shader program!");
  glDeleteProgram(prog);
}

void ShaderProgram::Attach(const Shader& shader) { Attach(shader.GetHandle()); }

void ShaderProgram::Attach(GLuint shader) {
  assert(prog != 0 && "Attempt to attach shader to an invalid program!");
  assert(shader != 0 && "Attemp to attach invalid shader to program!");
  glAttachShader(prog, shader);
}

void ShaderProgram::BindFragmentLocation(const char* attr, int color) {
  assert(prog != 0 && "Attempt to bind fragment location for invalid program!");
  glBindFragDataLocation(prog, color, attr);
}

bool ShaderProgram::Link() {
  assert(prog != 0 && "Attempt to link invalid program!");
  glLinkProgram(prog);

  GLint status;
  glGetProgramiv(prog, GL_LINK_STATUS, &status);
  // TODO: capture compile log and display it with some logging system
  assert(status == GL_TRUE && "Unable to link program!");
  return status == GL_TRUE;
}

void ShaderProgram::Use() {
  assert(prog != 0 && "Attempt to use invalid program!");
  glUseProgram(prog);
}

void ShaderProgram::VertexAttribPointerf(GLuint vao, const char* attr,
                                         int index, GLint size, bool normalized,
                                         GLsizei groupSize) {
  assert(prog != 0 &&
         "Attempt to attribute vertex pointer to invalid program!");
  const GLint loc = glGetAttribLocation(prog, attr);
  assert(loc >= 0 && "Unable to find attribute location!");
  glEnableVertexArrayAttrib(vao, loc);
  glVertexAttribPointer(loc, size, GL_FLOAT, normalized,
                        groupSize * sizeof(GLfloat),
                        (void*)(index * sizeof(GLfloat)));
}

void ShaderProgram::VertexAttribPointerf(const VertexArray& vao,
                                         const char* attr, int index,
                                         GLint size, bool normalized,
                                         GLsizei groupSize) {
  VertexAttribPointerf(vao.GetHandle(), attr, index, size, normalized,
                       groupSize);
}

template <class T>
void ShaderProgram::SetUniform(const char* name, const T& value) {
  SetUniform(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform(GLint loc, float value) {
  assert(prog != 0 && "Attempt to set uniform of an invalid program!");
  assert(loc >= 0 && "Attempt to set uniform for an invalid location!");
  glUniform1f(loc, value);
}

void ShaderProgram::SetUniform(GLint loc, int value) {
  assert(prog != 0 && "Attempt to set uniform of an invalid program!");
  assert(loc >= 0 && "Attempt to set uniform for an invalid location!");
  glUniform1i(loc, value);
}

void ShaderProgram::SetUniform(GLint loc, const glm::mat4& value) {
  assert(prog != 0 && "Attempt to set uniform of an invalid program!");
  assert(loc >= 0 && "Attempt to set uniform for an invalid location!");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::SetUniform(GLint loc, const glm::vec3& value) {
  assert(prog != 0 && "Attempt to set uniform of an invalid program!");
  assert(loc >= 0 && "Attempt to set uniform for an invalid location!");
  glUniform3f(loc, value.x, value.y, value.z);
}

GLint ShaderProgram::GetUniformLocation(const char* name) {
  assert(prog != 0 && "Attempt to get uniform location of an invalid program!");
  const GLint loc = glGetUniformLocation(prog, name);
  assert(loc >= 0 && "Unable to find uniform location!");
  return loc;
}
}  // namespace gl
}  // namespace l3d