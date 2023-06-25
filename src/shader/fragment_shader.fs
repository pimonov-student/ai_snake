#version 440 core

in vec2 texture_coord;

out vec4 color;

uniform sampler2D our_texture;

void main()
{
	color = texture(our_texture, texture_coord);
}