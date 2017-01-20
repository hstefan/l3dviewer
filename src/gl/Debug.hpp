#pragma once

#include <glad/glad.h>
#include <iostream>

namespace l3d {
namespace gl {

inline unsigned CheckErrors() {
  GLuint error;
  unsigned numError = 0;
  while (error = glGetError()) {
    ++numError;
    std::cout << "glGetError returned " << error << '\n';
  }
  return numError;
}

}  // namespace gl
}  // namespace l3d