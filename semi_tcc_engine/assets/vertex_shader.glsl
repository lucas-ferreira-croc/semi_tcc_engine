#version 330 core

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec2 aTextureCoord;

//out vec3 mainColor;
out vec2 textureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	//gl_Position = vec4(aPos, 1.0);
	//mainClor = aColor;
	textureCoord = aTextureCoord;
}