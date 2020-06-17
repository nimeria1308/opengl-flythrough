#version 330 core
out vec4 FragColor;

uniform sampler2D water; //the texture containing the heightmap
in vec2 tex; //the texture coordinates calculated in the vertex shader
  
void main()
{
	//todo: use texture for the terrain
	FragColor = vec4(texture(water, tex).rgb, 1.0);
} 
