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
	// Пустой конструктор
	Texture();
	// Конструктор с путем
	Texture(GLchar* texture_path);
	// Деструктор
	~Texture();
};

#endif