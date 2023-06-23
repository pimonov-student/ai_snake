#pragma once

#ifndef SNAKE_H
#define SNAKE_H

// ���������� ���, ��� ���� ��� OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Snake
{
private:
	// ������� ������ ������
	struct Position
	{
		GLfloat x;
		GLfloat y;
	};
	// ���������� ��� �������� ������ ������
	Position pos;
	GLchar direction;
	GLfloat step;
public:
	Snake();
	// ���������� �������
	glm::mat4 snake_control(glm::mat4 model);
	GLchar get_direction();
	void set_direction(char new_direction);
};

#endif