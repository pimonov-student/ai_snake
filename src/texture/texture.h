#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include "stb_image.h"

class Texture
{
public:
	// Идентификатор шейдера
	GLuint texture;
	// Переменные размера и цветовых каналов
	int width, height, n_channels;
	// Конструктор
	Texture(GLchar* texture_path);
	// Деструктор
	~Texture();
};

#endif