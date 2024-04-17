/*
* Ex 4: Write a program for plotting the sinc() function. This function is given as: sinc(x) = sin(x) / x, where x ∈ [-10, 10].
*		Use the GL_LINE_STRIP primitive, a step size of 0.25 and remember that sinc(0) = 1.
*/

#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <shaderClass.h>

const int   gW_Width = 640;
const int   gW_Height = 360;
const char* gW_Title = "Butterfly curve";

std::vector<GLfloat>* generateVerts(GLfloat xmin, GLfloat xmax, GLfloat step);

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

	GLfloat xmin = -10.0f;
	GLfloat xmax = 10.0f;
	GLfloat step = 0.25f;

	// populate array with sinc values
	std::vector<GLfloat>* vertex_data = generateVerts(xmin, xmax, step);
	int VN = vertex_data->size() / 2; // number of vertices

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_data->size() * sizeof(GLfloat), vertex_data->data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	Shader defaultShader = Shader("default.vert", NULL);

	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
	glLineWidth(3);

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		defaultShader.Activate();
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, VN);
		glBindVertexArray(0);

		glfwSwapBuffers(win);
	}

	defaultShader.Delete();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return 0;
}

GLfloat sinc(GLfloat x) {
	if (x == 0.0f) return 1.0f;
	return glm::sin(x) / x;
}

std::vector<GLfloat>* generateVerts(GLfloat xmin, GLfloat xmax, GLfloat step) {
	GLfloat length = xmax - xmin;
	int N = ((int)length / step) + 1;

	std::vector<GLfloat>* array = new std::vector<GLfloat>(N * 2);

	GLfloat x = xmin;

	for (int i = 0; i < N; i++) {
		(*array)[(i * 2)] = x;
		(*array)[(i * 2) + 1] = sinc(x);

		x += step;
	}

	return array;
}