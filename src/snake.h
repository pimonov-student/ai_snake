#pragma once

#ifndef SNAKE_H
#define SNAKE_H

// Подключаем все, что надо для OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Прочие необходимые
#include <stdlib.h>
#include <time.h>
#include <vector>

// Структура под координаты
struct Position
{
	GLfloat x;
	GLfloat y;
	Position& operator=(const Position& other);
};

class Snake
{
private:
	// Начальная позиция
	Position default_pos;
	// Позиция головы змейки
	Position head_pos;
	// Позиция хвоста змейки
	Position tail_pos;
	// Позиция сегментов тела
	std::vector <Position> body_pos;
	// Позиция еды
	Position food_pos;
	// Направление головы змейки
	GLchar direction;
	// Степень двойки (размер сетки)
	GLuint size;
	// Шаг змейки
	GLfloat step;
	// Матрицы для отрисовки
	glm::mat4 head_model;
	glm::mat4 food_model;
public:
	Snake();
	// Управление змейкой
	void game_cycle();
	// get'ы
	GLchar get_direction();
	glm::mat4 get_head_model();
	glm::mat4 get_food_model();
	int get_body_pos_size();
	Position get_body_pos(int id);
	GLfloat get_step();
	// set'ы
	void set_direction(char new_direction);
};

#endif