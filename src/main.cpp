#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
	// initializes glfw
	int glfwInitRc = glfwInit();
	if (!glfwInitRc) {
		fprintf(stderr, "Failed to init GLFW.");
		return -1;
	}

	// sets up OpenGL-specific window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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
	
	// runs application loop
	while (!glfwWindowShouldClose(window)) {
		// sets OpenGL clear color
		glClearColor(1.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	return 0;
}
