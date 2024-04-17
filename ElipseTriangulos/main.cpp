/*
* Ex 8: Randomly generate a number of triangles inside a circle.
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

void generateEllipseVerts(GLfloat arr[], GLfloat cx, GLfloat cy, GLfloat width, GLfloat height, GLuint resolution);
void generateTriangleVerts(GLfloat points[], GLfloat width, GLfloat height, GLuint nvert);

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

	const GLuint EllipseResolution = 360;
	const GLuint NumVertices = 30; //Numero de vertices, deve ser divisivel por 6 ou pode sobrar pontos.
	const GLfloat width = 1.9f;
	const GLfloat height = 1.0f;

	GLfloat verts[EllipseResolution * 2] = { 0 };
	generateEllipseVerts(verts, 0.0f, 0.0f, width, height, EllipseResolution);

	GLfloat triangles[NumVertices] = { 0 };
	generateTriangleVerts(triangles, width, height, NumVertices);

	GLuint VBO_e, VBO_t, VAO_e, VAO_t;

	glGenBuffers(1, &VBO_e);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_e);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO_e);
	glBindVertexArray(VAO_e);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_e);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);



	glGenBuffers(1, &VBO_t);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_t);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO_t);
	glBindVertexArray(VAO_t);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_t);
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
		glBindVertexArray(VAO_e);
		glDrawArrays(GL_LINE_LOOP, 0, EllipseResolution);
		glBindVertexArray(VAO_t);
		glDrawArrays(GL_TRIANGLES, 0, NumVertices / 2);		//Numero de triangulos gerados = NumVertices/6.
		glBindVertexArray(0);

		glfwSwapBuffers(win);
	}

	defaultShader.Delete();
	glDeleteVertexArrays(1, &VAO_e);
	glDeleteVertexArrays(1, &VAO_t);
	glDeleteBuffers(1, &VBO_t);
	glDeleteBuffers(1, &VBO_e);
	glfwTerminate();

	return 0;
}

void generateEllipseVerts(GLfloat arr[], GLfloat cx, GLfloat cy, GLfloat width, GLfloat height, GLuint resolution) {
	GLfloat x, y;

	GLfloat step = 2 * glm::pi<float>() / resolution; // step size in radians
	GLfloat theta;

	GLuint i = 0;
	for (GLuint j = 0; j < resolution; j++) {
		theta = j * step;

		x = cx + width * glm::cos(theta);
		y = cy + height * glm::sin(theta);

		arr[i] = x;
		arr[i + 1] = y;

		i += 2;
	}
}

void generateTriangleVerts(GLfloat points[], GLfloat width, GLfloat height, GLuint nvert) {
	GLfloat x, y;
	GLfloat rho, phi;
	srand(time(0));

	GLuint i = 0;
	for (GLuint j = 0; j < nvert / 2; j++) {
		rho = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX);
		phi = static_cast<GLfloat>(rand()) / static_cast<GLfloat>(RAND_MAX) * (2 * glm::pi<float>());

		x = sqrt(rho) * cos(phi);
		y = sqrt(rho) * sin(phi);

		x *= width;
		y *= height;

		points[i] = x;
		points[i + 1] = y;

		i += 2;
	}
}