/*
* Ex 3: Alter the previous exercise to draw a 2D house. The house consists of a square (front wall composded of 2 triangles) and a triangle (roof).
*		The first 4 given points define the square, while the last 3 define the triangle. Use the GL_TRIANGLES primitive and construct 2 windows and a doof for the house.
*/

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "shaderClass.h"

const int   gW_Width = 640;
const int   gW_Height = 360;
const char* gW_Title = "Pontos";

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

	Shader defaultShader = Shader("default.vert", "default.frag");
	GLint fragBool = glGetUniformLocation(defaultShader.ID, "isWall");
	if (fragBool == -1) {
		std::cerr << "Uniform not found" << std::endl;
		defaultShader.Delete();
		glfwTerminate();
		return -1;
	}

	// we divide these points by 30 in the vertex shader
	GLfloat vertices[] = {
		// house
		 0.0f,  0.0f,
		20.0f,  0.0f,
		20.0f, 20.0f,
		 0.0f, 20.0f,
		10.0f, 25.0f,
		
		// door
		 4.0f,  0.0f,
		 4.0f, 12.5f,
		 8.0f,  0.0f,
		 8.0f, 12.5f
	};

	GLuint vert_indices[] = {
		// house
		0, 1, 2, // first triangle
		0, 2, 3, // second triangle
		
		2, 3, 4, // roof

		// door
		5, 6, 7,
		6, 7, 8
	};

	GLuint VAO, VBO, IBO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// index buffers
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vert_indices), vert_indices, GL_STATIC_DRAW);

	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
	
	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		defaultShader.Activate();

		glUniform1i(fragBool, 0);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glUniform1i(fragBool, 1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(9 * sizeof(GLuint)));
		glBindVertexArray(0);

		glfwSwapBuffers(win);
	}

	defaultShader.Delete();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
	glfwTerminate();

	return 0;
}