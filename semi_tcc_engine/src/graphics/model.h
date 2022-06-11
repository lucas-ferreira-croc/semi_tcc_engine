#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "mesh.h"
#include "shader.h"

class model
{
private:
public:
	std::vector<mesh> meshes;

	model();

	void init();

	void render(shader shader);
	void cleanup();
};

#endif // !MODEL_H
