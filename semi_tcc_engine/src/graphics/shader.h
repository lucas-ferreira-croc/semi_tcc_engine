#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


class shader 
{
private:
	unsigned int id;
	
	std::string loadSrc(const char* filepath);
	GLuint compile(const char* filepath, GLenum type, char* infoLog, int success);
public:
	shader();
	shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	void generate(const char* vertShaderPath, const char* fragShaderPath);
	void activate();

	// uniform functions
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void set3Float(const std::string& name, float value1, float value2, float value3);
	void set3Float(const std::string& name, glm::vec3 value);
	void set4Float(const std::string& name, float value1, float value2, float value3, float value4);
	void set4Float(const std::string& name, glm::vec4 value);
	void setMat3(const std::string& name, glm::mat3 value);
	void setMat4(const std::string& name, glm::mat4 value);
};

#endif