#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <streambuf>
#include <string>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

static bool compileShader(GLuint shader) {
  GLint status;
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  // if status == GL_FALSE, the compile failed
  if (status != GL_TRUE) {
    std::array<char, 512> buffer;
    glGetShaderInfoLog(shader, buffer.size(), nullptr, buffer.data());
    printf("Error compiling shader.\n%s", buffer.data());
  }
  return status == GL_TRUE;
}

static void checkGLError() {
  GLuint error = glGetError();
  if (error != 0) printf("glGetError returned %d!\n", error);
}

// extracted from
// https://insanecoding.blogspot.com.br/2011/11/how-to-read-in-file-in-c.html
std::string getFileContents(const char* filepath) {
  std::ifstream file(filepath, std::ios::in | std::ios::binary);
  if (file) {
    return std::string{std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>()};
  }
  return "";
}

GLuint createTextureFromFile(GLenum texture, const char* filename) {
  GLuint tex;
  glGenTextures(1, &tex);
  glActiveTexture(texture);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int w;
  int h;
  int c;
  unsigned char* texBytes = stbi_load(filename, &w, &h, &c, STBI_rgb);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
               texBytes);
  glGenerateMipmap(GL_TEXTURE_2D);
  checkGLError();
  stbi_image_free(texBytes);
  return tex;
}

int main() {
  // initializes glfw
  int glfwInitRc = glfwInit();
  if (!glfwInitRc) {
    fprintf(stderr, "Failed to init GLFW.");
    return -1;
  }

  // sets up OpenGL-specific window hints
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // create glfw window
  GLFWwindow* window = glfwCreateWindow(1280, 720, "L3Viewer", nullptr, nullptr);
  if (!window) {
    fprintf(stderr, "Failed to create window.");
    glfwTerminate();
    return -1;
  }

  // sets up window as our OpenGL context
  glfwMakeContextCurrent(window);

  // configures glad to load using glfw utility
  int gladLoadRc = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  if (!gladLoadRc) {
    fprintf(stderr, "Failed to load OpenGL with glad: error code %d\n",
            gladLoadRc);
    return -1;
  }

  printf("OpenGL version: %d.%d\n", GLVersion.major, GLVersion.minor);

  // generates VAO to avoid having to reconfigure attributes every time we
  // switch the active shader program
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  checkGLError();

  // uploads vertex data to GPU buffers (VBOs)
  std::array<GLfloat, 28> vertices = {
      -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
      -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
  };
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
               vertices.data(), GL_STATIC_DRAW);
  checkGLError();

  // creates the vertex shader
  const std::string vertexSource = getFileContents("files/vertex.glsl");
  const GLchar* vertexSrc = vertexSource.c_str();
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
  compileShader(vertexShader);
  checkGLError();

  // creates the fragment shader
  const std::string fragSource = getFileContents("files/frag.glsl");
  const GLchar* fragSrc = fragSource.c_str();
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, &fragSrc, nullptr);
  compileShader(fragShader);
  checkGLError();

  // create a shader program with both shaders from above
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);
  GLint programStatus;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programStatus);
  if (programStatus != GL_TRUE) {
    std::array<char, 512> buffer;
    glGetProgramInfoLog(shaderProgram, buffer.size(), nullptr, buffer.data());
    printf("Failed to link shader program.\n%s", buffer.data());
  }

  glUseProgram(shaderProgram);
  checkGLError();

  // sets up position attribute for the shader program
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
                        0);
  checkGLError();

  // sets color uniform
  GLint color = glGetAttribLocation(shaderProgram, "color");
  glEnableVertexAttribArray(color);
  glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
                        (void*)(2 * sizeof(GLfloat)));
  checkGLError();

  // sets color uniform
  GLint texCoord = glGetAttribLocation(shaderProgram, "texCoord");
  glEnableVertexAttribArray(texCoord);
  glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
                        (void*)(5 * sizeof(GLfloat)));
  checkGLError();

  // finds time uniform
  GLint timeSinceStart = glGetUniformLocation(shaderProgram, "timeSinceStart");
  glUniform1f(timeSinceStart, 0.f);
  checkGLError();

  // create ebo
  std::array<GLuint, 6> indices = {0, 1, 2, 2, 3, 0};
  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  GLuint helloTex = createTextureFromFile(GL_TEXTURE0, "files/hello.png");
  GLuint baconTex = createTextureFromFile(GL_TEXTURE1, "files/bacon.png");

  glUniform1i(glGetUniformLocation(shaderProgram, "texPepper"), 0);
  glUniform1i(glGetUniformLocation(shaderProgram, "texBacon"), 1);

  GLint trans = glGetUniformLocation(shaderProgram, "trans");

  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  const auto start = high_resolution_clock::now();

  // runs application loop
  while (!glfwWindowShouldClose(window)) {
    // sets OpenGL clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // updates time uniform attribute
    const auto timeDelta = high_resolution_clock::now() - start;
    const float time = duration_cast<duration<float>>(timeDelta).count();
    glUniform1f(timeSinceStart, time);

    // sets up transformation matix
    glm::mat4 transMat;
    transMat = glm::rotate(transMat,
        time * glm::radians(45.0f),
        glm::vec3(1.0f, 1.0f, 0.5f));
    glUniformMatrix4fv(trans, 1, GL_FALSE, glm::value_ptr(transMat));

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    checkGLError();

    glfwSwapBuffers(window);
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
  }

  // cleans up shaders
  glDeleteProgram(shaderProgram);
  glDeleteShader(fragShader);
  glDeleteShader(vertexShader);

  // clean up vbo
  glDeleteBuffers(1, &vbo);

  // clean up vao
  glDeleteVertexArrays(1, &vao);

  return 0;
}
