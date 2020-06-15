#version 330 core
out vec4 FragColor;

in vec2 tex; //the texture coordinates calculated in the vertex shader
  
void main()
{
	//todo: use texture for the terrain
	FragColor = vec4(0.5, 0.5, 0.5, 1.0);
} 


