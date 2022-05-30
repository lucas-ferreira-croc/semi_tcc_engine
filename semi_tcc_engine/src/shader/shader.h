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

#endif // SHADER_H
