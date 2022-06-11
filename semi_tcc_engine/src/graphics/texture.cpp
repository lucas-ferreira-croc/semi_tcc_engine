#include "texture.h"
#include <iostream>
int texture::currentId = 0;

texture::texture()
{
}

texture::texture(const char* path, const char* name, bool defaultParams)
	:path(path), name(name), id(currentId++)
{
	generate();
	if (defaultParams) {
		setFilters(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		setWrap(GL_REPEAT);
	}
}

void texture::generate()
{
	glGenTextures(1, &text);
	glBindTexture(GL_TEXTURE_2D, text);
}

void texture::load(bool flip)
{
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);
	GLenum colorMode = GL_RGB;
	
	switch (nChannels) {
		case 1: 
			colorMode = GL_RED;
			break;
		case 4:
			colorMode = GL_RGBA;
			break;
	};
	
	if (data) {
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture " << name << std::endl;
	}
	stbi_image_free(data);
}

void texture::setFilters(GLenum all)
{
	setFilters(all, all);
}

void texture::setFilters(GLenum mag, GLenum min)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
}

void texture::setWrap(GLenum all)
{
	setWrap(all, all);
}

void texture::setWrap(GLenum s, GLenum t)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
}

void texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}
