#version 440 core

in vec2 texture_coord;

out vec4 color;

uniform sampler2D texture_one;
uniform sampler2D texture_two;

void main()
{
	color = mix(texture(texture_one, texture_coord), texture(texture_two, texture_coord), 0.2);
}