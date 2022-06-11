#include "model.h"

model::model()
{
}

void model::init()
{

}

void model::render(shader shader)
{
	for (mesh mesh : meshes) {
		mesh.render(shader);
	}
}

void model::cleanup(){
	for (mesh mesh : meshes) {
		mesh.cleanup();
	}
}
