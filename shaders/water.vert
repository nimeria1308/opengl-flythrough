#version 330 core
layout (location = 0) in vec3 aPos;

out vec2 tex; //the calculated coordinates for the heightmap texture for this vertex

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float time;

const float amplitude = 0.125;
const float frequency = 1.2;
const float PI = 3.14159;

uniform float length; //length of the terrain - z
uniform float width;  //width of the terrain - x

void main()
{
    float distance = length(aPos);
    float y = amplitude*sin(-PI*distance*frequency+time);

	tex = vec2(aPos.x / width, aPos.z / length);
	vec3 pos = vec3(aPos.x, y * 10, aPos.z);
    gl_Position = proj * view * model * vec4(pos, 1.0);
}
