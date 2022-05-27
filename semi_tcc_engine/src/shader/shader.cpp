#include "shader.h"

shader::shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	int success = 0;
	char infoLog[512];

	GLuint vertexShader = compile(vertexShaderPath, GL_VERTEX_SHADER, infoLog, success);
	GLuint fragmentShader = compile(fragmentShaderPath, GL_FRAGMENT_SHADER, infoLog, success);

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);

	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "Linking error: " << std::endl << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

std::string shader::loadSrc(const char* filepath)
{
	std::ifstream file;
	std::stringstream buffer;

	std::string source = "";

	file.open(filepath);
	if(file.is_open())
	{
		buffer << file.rdbuf();
		source = buffer.str();
	}
	else
	{
		std::cout << "Could not open: " << filepath << std::endl;
	}

	file.close();
	return source;
}

GLuint shader::compile(const char* filepath, GLenum type, char* infoLog, int success)
{
	unsigned int shader = glCreateShader(type);
	std::string loaded_source = loadSrc(filepath);
	const GLchar* source = loaded_source.c_str();
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Errror compiling shader: " << std::endl << infoLog << std::endl;
	}

	return shader;
}

void shader::activate()
{
	glUseProgram(id);
}

void shader::setMat4(const std::string& name, glm::mat4 value)
{
	activate();
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}