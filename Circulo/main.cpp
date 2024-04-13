#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

#include <shaderClass.h>

const int   gW_Width = 640;
const int   gW_Height = 360;
const char* gW_Title = "Circulo";

void generateCircleVerts(GLfloat arr[], GLfloat cx, GLfloat cy, GLfloat r, GLuint resolution);

int main(void) {
	// init glfw
	if (!glfwInit()) {
		std::cerr << "GLFW initialization failed with error code " << glfwGetError(NULL) << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* win = glfwCreateWindow(gW_Width, gW_Height, gW_Title, NULL, NULL);
	if (!win) {
		std::cerr << "GLFW window creating failed with error code " << glfwGetError(NULL) << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(win);

	// init glew
	glewExperimental = GL_TRUE;
	if (GLenum err = glewInit() != GLEW_OK) {
		std::cerr << "GLEW initialization failed with error code " << err << std::endl;
		return -1;
	}

	const GLuint CircleResolution = 360;
	GLfloat verts[CircleResolution * 2] = { 0 };
	generateCircleVerts(verts, 0.0f, 0.0f, 1.0f, CircleResolution);

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	Shader defaultShader = Shader("default.vert", NULL);

	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
	glLineWidth(2);

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		defaultShader.Activate();
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_LOOP, 0, CircleResolution);
		glBindVertexArray(0);

		glfwSwapBuffers(win);
	}

	defaultShader.Delete();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return 0;
}

void generateCircleVerts(GLfloat arr[], GLfloat cx, GLfloat cy, GLfloat r, GLuint resolution) {
	GLfloat x, y;

	GLfloat step = 2 * glm::pi<float>() / resolution; // step size in radians
	GLfloat theta;

	GLuint i = 0;
	for (GLuint j = 0; j < resolution; j++) {
		theta = j * step;

		x = cx + r * glm::cos(theta);
		y = cy + r * glm::sin(theta);

		arr[i] = x;
		arr[i + 1] = y;

		i += 2;
	}
}