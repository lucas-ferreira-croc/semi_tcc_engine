#version 330 core 

out vec4 FragColor;
in vec3 mainColor;
in vec2 textureCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform float mixRatio;

void main()
{
	FragColor = mix(texture(texture0, textureCoord), texture(texture1, textureCoord), mixRatio);
}