/*
* Ex 6: Randomly generate a number of points inside a circle.
*/

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

#include <shaderClass.h>

const int   gW_Width = 640;
const int   gW_Height = 360;
const char* gW_Title = "ElipseLinhas";

void generateCircleVerts(GLfloat arr[], GLfloat cx, GLfloat cy, GLfloat r, GLuint resolution);
void generatePointVerts(GLfloat points[], GLfloat r, GLuint resolution);

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

	GLfloat points[CircleResolution] = { 0 };
	generatePointVerts(points, 1.0f, CircleResolution);

	GLuint VBO_c, VBO_p, VAO_c, VAO_p;

	glGenBuffers(1, &VBO_c);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_c);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO_c);
	glBindVertexArray(VAO_c);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_c);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);



	glGenBuffers(1, &VBO_p);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_p);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO_p);
	glBindVertexArray(VAO_p);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_p);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	Shader defaultShader = Shader("default.vert", NULL);

	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
	glLineWidth(2);
	glPointSize(5);

	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		defaultShader.Activate();
		glBindVertexArray(VAO_c);
		glDrawArrays(GL_LINE_LOOP, 0, CircleResolution);
		glBindVertexArray(VAO_p);
		glDrawArrays(GL_POINTS, 0 , CircleResolution/2);
		glBindVertexArray(0);

		glfwSwapBuffers(win);
	}

	defaultShader.Delete();
	glDeleteVertexArrays(1, &VAO_c);
	glDeleteVertexArrays(1, &VAO_p);
	glDeleteBuffers(1, &VBO_p);
	glDeleteBuffers(1, &VBO_c);
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

void generatePointVerts(GLfloat points[], GLfloat r, GLuint resolution) {
	GLfloat x, y;
	GLfloat rho, phi;
	srand(time(0));

	GLuint i = 0;
	for (GLuint j = 0; j < resolution / 2; j++) {
		rho = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX);
		phi = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX) * (2 * glm::pi<float>());

		x = sqrt(rho) * cos(phi);
		y = sqrt(rho) * sin(phi);

		x *= r;
		y *= r;

		points[i] = x;
		points[i + 1] = y;

		i += 2;
	}
}