#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "shaderClass.h"

const int   gW_Width = 640;
const int   gW_Height = 360;
const char* gW_Title = "Folium of Descartes";

std::vector<GLfloat>* generateVerts(GLfloat tmin, GLfloat tmax, GLfloat a, GLfloat step);

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

	GLfloat tmin = -10.0f;
	GLfloat tmax = 10.0f;
	GLfloat a = 1.0f;
	GLfloat step = 0.01f;

	std::vector<GLfloat>* vertex_data = generateVerts(tmin, tmax, a, step);
	int VN = vertex_data->size() / 2;

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

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		defaultShader.Activate();
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, VN);
		glBindVertexArray(0);

		glfwSwapBuffers(win);
	}

	glfwTerminate();

	return 0;
}

std::pair<GLfloat, GLfloat> folium(GLfloat a, GLfloat t) {
	if (t == -1) return std::make_pair(-1.0f, 0.3);

	GLfloat x = (3 * a * t) / (1 + powf(t, 3));
	GLfloat y = (3 * a * powf(t, 2)) / (1 + powf(t, 3));

	return std::make_pair(x, y);
}

std::vector<GLfloat>* generateVerts(GLfloat tmin, GLfloat tmax, GLfloat a, GLfloat step) {
	GLfloat length = tmax - tmin;
	int N = ((int)length / step) + 1;

	std::vector<GLfloat>* array = new std::vector<GLfloat>(N * 2);

	GLfloat t = tmin;

	for (int i = 0; i < N; i++) {
		std::pair<GLfloat, GLfloat> point = folium(a, t);

		(*array)[(i * 2)] = point.first;
		(*array)[(i * 2) + 1] = point.second;

		t += step;
	}

	return array;
}