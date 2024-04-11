#include "shaderClass.h"

std::string get_file_contents(const GLchar* filename) {
	if (!filename) return NULL;

	std::string content;
	std::ifstream fileStream(filename, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filename << ". File does not exist." << std::endl;
		return NULL;
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	std::cout << "-" << std::endl << content << std::endl << "-" << std::endl;
	return content;
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	if (!fragmentFile) {
		vertexCode = get_file_contents(vertexFile);
		const char* vertexSource = vertexCode.c_str();

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertexSource, NULL);
		glCompileShader(vs);

		ID = glCreateProgram();
		glAttachShader(ID, vs);

		glLinkProgram(ID);

		glDeleteShader(vs);
		return;
	}

	vertexCode = get_file_contents(vertexFile);
	fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexSource, NULL);
	glCompileShader(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentSource, NULL);
	glCompileShader(fs);

	ID = glCreateProgram();
	glAttachShader(ID, vs);
	glAttachShader(ID, fs);
	glLinkProgram(ID);

	glDeleteShader(vs);
	glDeleteShader(fs);
}

void Shader::Activate() {
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}