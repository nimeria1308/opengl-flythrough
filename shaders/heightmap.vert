#version 330 core
layout (location = 0) in vec3 aPos;

out vec2 tex; //the calculated coordinates for the heightmap texture for this vertex

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform sampler2D heightmap; //the texture containing the heightmap

uniform float length; //length of the terrain - z
uniform float width;  //width of the terrain - x

void main()
{
	//todo: calculate the correct texture coordinate for the vertex
	//take care - if the step between the vertices is not 1
	tex = vec2(aPos.x / width, aPos.z / length);
	//todo: get the height (y) from the texture
	//eventually add a coefficient for the height (otherwise it will be between [0;1])
	float height = texture(heightmap, tex).r;
	vec3 pos = vec3(aPos.x, height * 200, aPos.z);
    
    gl_Position = proj * view * model * vec4(pos, 1.0);
}

