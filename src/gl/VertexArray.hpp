#pragma once

#include <glad/glad.h>
#include <cassert>

namespace l3d {
namespace gl {

class VertexArray {
 public:
  inline VertexArray();
  inline ~VertexArray();
  inline void Bind();
  inline GLuint GetHandle() const;

 private:
  GLuint vao;
};

VertexArray::VertexArray() : vao(0) {
  glGenVertexArrays(1, &vao);
  assert(vao != 0 && "Unable to generate vertex array!");
}

VertexArray::~VertexArray() {
  assert(vao != 0 && "Attempt to destroy empty vertex array.");
  glDeleteVertexArrays(1, &vao);
}

void VertexArray::Bind() {
  assert(vao != 0 && "Attempt to bind empty vertex array.");
  glBindVertexArray(vao);
}

GLuint VertexArray::GetHandle() const { return vao; }

}  // namespace gl
}  // namespace l3d