#version 330 core 

out vec4 FragColor;
in vec3 mainColor;

void main()
{
	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);	
	FragColor = vec4(mainColor, 1.0);
}