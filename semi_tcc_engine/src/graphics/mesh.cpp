#include "mesh.h"

std::vector<vertex> vertex::genList(float* vertices, int nVertices) {
	std::vector<vertex> ret(nVertices);

	int stride = sizeof(vertex) / sizeof(float);

	for (int i = 0; i < nVertices; i++) {
		ret[i].pos = glm::vec3(
			vertices[i * stride + 0],
			vertices[i * stride + 1],
			vertices[i * stride + 2]
			);

		ret[i].texCoord = glm::vec2(
			vertices[i * stride + 3],
			vertices[i * stride + 4]
			);
	}
	
	return ret;
}

mesh::mesh()
{
}

mesh::mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<texture> textures)
	: vertices(vertices), indices(indices), textures(textures)
{
	setUp();
}

void mesh::render(shader shader)
{
	for(unsigned int i = 0; i < textures.size(); i++) {
		shader.setInt(textures[i].name, textures[i].id);
		glActiveTexture(GL_TEXTURE0 + i);
		
		textures[i].bind();
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void mesh::cleanup()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void mesh::setUp()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//set vertex attrib pointers
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, pos));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) offsetof(vertex, texCoord));

	glBindVertexArray(0);
}
