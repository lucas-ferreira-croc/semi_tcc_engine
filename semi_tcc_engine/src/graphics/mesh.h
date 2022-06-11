#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include "shader.h"
#include "texture.h"

struct vertex 
{
	glm::vec3 pos;
	glm::vec2 texCoord;

	static std::vector<struct vertex> genList(float* vertices, int nVertices);
};

class mesh
{
private:
	unsigned int VBO;
	unsigned int EBO;

	void setUp();
public:
	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;
	
	unsigned int VAO;
	std::vector<texture> textures;

	mesh();
	mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<texture> textures);

	void render(shader shader);
	void cleanup();
};
#endif // !MESH_H
