#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "shaderClass.h"

const int   gW_Width = 640;
const int   gW_Height = 360;
const char* gW_Title = "2 triangulos";

int main(void) {
	// init glfw
	if (!glfwInit()) {
		std::cerr << "GLFW initialization failed with error code " << glfwGetError(NULL) << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *win = glfwCreateWindow(gW_Width, gW_Height, gW_Title, NULL, NULL);
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

	GLfloat vert_pos[] = {
		// triangulo 1
		-0.5f,   0.75f, 0.0f, // topo
		-0.25f, -0.25f, 0.0f, // direita
		-0.75f, -0.25f, 0.0f, // esquerda

		// triangulo 2
		 0.5f,   0.75f, 0.0f,
		 0.75f, -0.25f, 0.0f,
		 0.25f, -0.25f, 0.0f,
	};

	GLfloat vert_col[] = {
		// triangulo 2
		1.0f, 0.0f, 0.0f, // topo
		1.0f, 0.0f, 0.0f, // direita
		1.0f, 0.0f, 0.0f, // esquerda

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	GLuint VBO_p, VBO_c, VAO;

	glGenBuffers(1, &VBO_p);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_p);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert_pos), vert_pos, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO_c);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_c);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert_col), vert_col, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_p);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); // position attrib
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_c);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL); // color attrib
	glEnableVertexAttribArray(1);

	Shader defaultShader = Shader("default.vert", "default.frag");

	glDisable(GL_POLYGON_SMOOTH);

	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);
		
		defaultShader.Activate();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(win);
	}

	defaultShader.Delete();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO_p);
	glDeleteBuffers(1, &VBO_c);
	glfwTerminate();

	return 0;
}