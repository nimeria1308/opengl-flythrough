#version 330 core
out vec4 FragColor;

uniform sampler2D heightcolor; //the texture containing the heightmap
in vec2 tex; //the texture coordinates calculated in the vertex shader
  
void main()
{
	//todo: use texture for the terrain
	FragColor = vec4(texture(heightcolor, tex).rgb, 1.0);
} 
