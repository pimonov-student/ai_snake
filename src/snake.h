#pragma once

#ifndef SNAKE_H
#define SNAKE_H

// ���������� ���, ��� ���� ��� OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ������ �����������
#include <stdlib.h>
#include <time.h>
#include <vector>

// ��������� ��� ����������
struct Position
{
	GLfloat x;
	GLfloat y;
	Position& operator=(const Position& other);
};

class Snake
{
private:
	// ��������� �������
	Position default_pos;
	// ������� ������ ������
	Position head_pos;
	// ������� ������ ������
	Position tail_pos;
	// ������� ��������� ����
	std::vector <Position> body_pos;
	// ������� ���
	Position food_pos;
	// ����������� ������ ������
	GLchar direction;
	// ������� ������ (������ �����)
	GLuint size;
	// ��� ������
	GLfloat step;
	// ������� ��� ���������
	glm::mat4 head_model;
	glm::mat4 food_model;
public:
	Snake();
	// ���������� �������
	void game_cycle();
	// get'�
	GLchar get_direction();
	glm::mat4 get_head_model();
	glm::mat4 get_food_model();
	int get_body_pos_size();
	Position get_body_pos(int id);
	GLfloat get_step();
	// set'�
	void set_direction(char new_direction);
};

#endif