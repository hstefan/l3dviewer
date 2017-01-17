#pragma once

#include <glad/glad.h>
#include <cassert>

namespace l3d {
namespace gl {
class VertexBuffer {
 public:
  inline VertexBuffer();
  inline ~VertexBuffer();
  inline void Bind();
  template <class T>
  inline void Data(const T& buffer);

 private:
  GLuint vbo;
};

VertexBuffer::VertexBuffer() : vbo(0) {
  glGenBuffers(1, &vbo);
  assert(vbo != 0 && "Unable to generate vertex buffer!");
}

VertexBuffer::~VertexBuffer() {
  assert(vbo != 0 && "Attempt to destroy an invalid vertex buffer!");
  glDeleteBuffers(1, &vbo);
}

void VertexBuffer::Bind() {
  assert(vbo != 0 && "Attempt to bind an invalid vertex buffer!");
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

template <class T>
void VertexBuffer::Data(const T& buffer) {
  Bind();
  glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(T::value_type),
               buffer.data(), GL_STATIC_DRAW);
}

} // namespace gl
} // namespace l3d