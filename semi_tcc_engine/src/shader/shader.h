#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class shader
{
private:
	unsigned int id;

	// utility
	std::string loadSrc(const char* filepath);
	GLuint compile(const char* filepath, GLenum type, char* infoLog, int sucess);

public:
	shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	void activate();

	//uniform function
	void setMat4(const std::string& name, glm::mat4 value);
	
};

#endif // SHADER_H
