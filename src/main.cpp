﻿#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include <chrono>

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
  if (error != 0)
	printf("glGetError returned %d!\n", error);
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
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, TRUE);

  // create glfw window
  GLFWwindow* window = glfwCreateWindow(640, 480, "L3Viewer", nullptr, nullptr);
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
  std::array<float, 6> vertices = {
	  0.0f, 0.5f,
	  0.5f, -0.5f,
	  -0.5f, -0.5f
  };
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
  checkGLError();

  // creates the vertex shader from a hardcoded string
  const GLchar* vertexSource =
	"#version 150\n"
	"in vec2 position;"
	"void main() {"
	"	gl_Position = vec4(position, 0.0, 1.0);"
	"}";
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, nullptr);
  compileShader(vertexShader);
  checkGLError();

  // creates the fragment shader from a hardcoded string
  const GLchar* fragSource =
	"#version 150\n"
	"uniform vec3 triangleColor;"
	"uniform float timeSinceStart;"
	"out vec4 outColor;"
	"void main() {"
	"	float t = (sin(timeSinceStart * 4.0) + 1.0) * 0.5;"
	"	outColor = vec4(t * triangleColor, 1.0);"
	"}";
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, &fragSource, nullptr);
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
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  checkGLError();

  // sets color uniform
  GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
  glUniform3f(uniColor, 1.f, 1.f, 0.f);
  checkGLError();

  // finds time uniform
  GLint timeSinceStart = glGetUniformLocation(shaderProgram, "timeSinceStart");
  glUniform1f(timeSinceStart, 0.f);
  checkGLError();

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

	glDrawArrays(GL_TRIANGLES, 0, 3);
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