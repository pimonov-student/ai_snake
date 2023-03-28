#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>

class Shader
{
public:
	// Идентификатор программы (?)
	GLuint program;
	// Конструктор, считывающий и собирающий шейдеры (вершинный и фрагментный)
	Shader(const GLchar* vertex_path, const GLchar* fragment_path);
	// Деструктор
	~Shader();
	// Проверка сборки шейдера
	void check_shader_compilation(GLuint shader);
	// Проверка линковки программы
	void check_program_linking(GLuint program);
	// Использование программы
	void use();
};

#endif