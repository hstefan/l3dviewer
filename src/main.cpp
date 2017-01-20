#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <array>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <streambuf>
#include <string>
#include "gl/Debug.hpp"
#include "gl/Shader.hpp"
#include "gl/ShaderProgram.hpp"
#include "gl/VertexArray.hpp"
#include "gl/VertexBuffer.hpp"
#include "gl/Window.hpp"

static bool compileShader(GLuint shader) {
  GLint status;
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  // if status == GL_FALSE, the compile failed
  if (status != GL_TRUE) {
    std::array<char, 512> buffer;
    glGetShaderInfoLog(shader, static_cast<GLsizei>(buffer.size()), nullptr,
                       buffer.data());
    printf("Error compiling shader.\n%s", buffer.data());
  }
  return status == GL_TRUE;
}

GLuint createTextureFromFile(GLenum texture, const char* filename) {
  GLuint tex;
  glGenTextures(1, &tex);
  glActiveTexture(texture);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int w;
  int h;
  int c;
  unsigned char* texBytes = stbi_load(filename, &w, &h, &c, STBI_rgb);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
               texBytes);
  glGenerateMipmap(GL_TEXTURE_2D);
  l3d::gl::CheckErrors();
  stbi_image_free(texBytes);
  return tex;
}

int main() {
  // create glfw window
  const int winWidth = 1280;
  const int winHeight = 720;
  const l3d::gl::ContextProperties ctx{4, 5, true, true};
  l3d::gl::Window window("L3Dviewer", winWidth, winHeight, ctx);

  // generates VAO to avoid having to reconfigure attributes every time we
  // switch the active shader program
  l3d::gl::VertexArray vao;
  vao.Bind();

  // clang-format off
  // uploads vertex data to GPU buffers (VBOs)
  std::array<GLfloat, 336> vertices = {
     -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    // vertices for reflective plane
    -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
  };
  // clang-format on

  l3d::gl::VertexBuffer vbo;
  vbo.Bind();
  vbo.Data(vertices);
  l3d::gl::CheckErrors();

  // creates the vertex shader
  l3d::gl::Shader vertexShader(GL_VERTEX_SHADER);
  vertexShader.SourceFromFile("files/cube.vert");
  vertexShader.Compile();
  l3d::gl::CheckErrors();

  // creates the fragment shader
  l3d::gl::Shader fragShader(GL_FRAGMENT_SHADER);
  fragShader.SourceFromFile("files/cube.frag");
  fragShader.Compile();
  l3d::gl::CheckErrors();

  // create a shader program with both shaders from above
  l3d::gl::ShaderProgram prog;
  prog.Attach(vertexShader);
  prog.Attach(fragShader);
  prog.BindFragmentLocation("outColor", 0);
  prog.Link();
  prog.Use();
  l3d::gl::CheckErrors();

  // sets up position attribute for the shader program
  prog.VertexAttribPointerf(vao, "position", 0, 3, false, 8);
  l3d::gl::CheckErrors();

  // sets color uniform
  prog.VertexAttribPointerf(vao, "color", 3, 3, false, 8);
  l3d::gl::CheckErrors();

  // sets color uniform
  prog.VertexAttribPointerf(vao, "texCoord", 6, 2, false, 8);
  l3d::gl::CheckErrors();

  // finds time uniform
  const GLint timeLoc = prog.GetUniformLocation("timeSinceStart");
  prog.SetUniform(timeLoc, 0.f);
  l3d::gl::CheckErrors();

  GLuint helloTex = createTextureFromFile(GL_TEXTURE0, "files/hello.png");
  GLuint baconTex = createTextureFromFile(GL_TEXTURE1, "files/bacon.png");

  prog.SetUniform("texPepper", 0);
  prog.SetUniform("texBacon", 1);

  GLint modelUni = prog.GetUniformLocation("model");

  GLint viewUni = prog.GetUniformLocation("view");
  glm::mat4 view =
      glm::lookAt(glm::vec3(1.2f, 2.2f, 1.4f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f));
  prog.SetUniform(viewUni, view);

  GLint projUni = prog.GetUniformLocation("proj");
  const float aspectRatio =
      static_cast<float>(winWidth) / static_cast<float>(winHeight);
  glm::mat4 proj =
      glm::perspective(glm::radians(45.0f), aspectRatio, 1.0f, 10.f);
  prog.SetUniform(projUni, proj);

  GLint colorUni = prog.GetUniformLocation("overrideColor");

  // enable depth test
  glEnable(GL_DEPTH_TEST);

  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  const auto start = high_resolution_clock::now();

  // variables necessary for rotating object when we press space
  const float xMaxAngSpeed = 180.0f;
  const float xAngAccel = 45.0f;
  float xAngSpeed = 0.0f;
  float xAng = 0.0f;

  // time from previous frame
  auto lastFrameTime = high_resolution_clock::now();
  float deltaTime = 0.0f;

  // runs application loop
  while (window.IsOpen()) {
    // sets OpenGL clear color
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // updates time uniform attribute
    const auto totalTime = high_resolution_clock::now() - start;
    const float time = duration_cast<duration<float>>(totalTime).count();
    prog.SetUniform(timeLoc, time);

    // updates deltaTime
    const auto frameDuration = high_resolution_clock::now() - lastFrameTime;
    deltaTime = duration_cast<duration<float>>(frameDuration).count();
    lastFrameTime = high_resolution_clock::now();

    // sets up transformation matix
    glm::mat4 model;

    // constantly rotates the object on the Z axis
    model = glm::rotate(model, time * glm::radians(45.0f),
                        glm::vec3(0.0f, 0.0f, 1.0f));

    // smoothly rotates the object on the X axis based on user's input
    model = glm::rotate(model, glm::radians(xAng), glm::vec3(1.0f, 0.0f, 0.0f));

    // uploads model matrix to our uniform attribute
    prog.SetUniform(modelUni, model);

    // reset color override
    prog.SetUniform(colorUni, glm::vec3(1.0f, 1.0f, 1.0f));

    // draw cube
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // enables stencil test to implement planar reflections
    glEnable(GL_STENCIL_TEST);

    // draw plane
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);
    glDepthMask(GL_FALSE);
    glClear(GL_STENCIL_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 36, 6);

    // draw cube reflection
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDepthMask(GL_TRUE);

    model = glm::scale(glm::translate(model, glm::vec3(0, 0, -1)),
                       glm::vec3(1, 1, -1));
    prog.SetUniform(modelUni, model);

    prog.SetUniform(colorUni, glm::vec3(0.3f, 0.3f, 0.3f));

    glDrawArrays(GL_TRIANGLES, 0, 36);

    l3d::gl::CheckErrors();

    glDisable(GL_STENCIL_TEST);

    window.SwapBuffers();

    window.PollEvents();
    // controls angular speed for the object x rotation
    if (window.IsKeyPressed(GLFW_KEY_SPACE))
      xAngSpeed += xAngAccel * deltaTime;
    else
      xAngSpeed -= xAngAccel * deltaTime;

    xAngSpeed = glm::clamp(xAngSpeed, 0.0f, xMaxAngSpeed);
    xAng += xAngSpeed * deltaTime;

    if (window.IsKeyPressed(GLFW_KEY_ESCAPE)) window.Close();
  }

  return 0;
}
