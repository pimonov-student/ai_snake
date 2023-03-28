#version 440 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texture;

out vec2 texture_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	texture_coord = texture;
}