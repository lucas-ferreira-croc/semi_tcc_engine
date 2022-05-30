#version 330 core 

out vec4 FragColor;
in vec3 mainColor;
in vec2 textureCoord;

uniform sampler2D textureOne;
uniform sampler2D textureTwo;

void main()
{
	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);	
	//FragColor = vec4(mainColor, 1.0);
	//FragColor = vec4(mainColor, 1.0f) * texture(textureOne, textureCoord);
	FragColor = mix(texture(textureOne, textureCoord), texture(textureTwo, textureCoord), 0.5);
}