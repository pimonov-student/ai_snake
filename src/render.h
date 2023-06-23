#pragma once

#ifndef RENDER_H
#define RENDER_H

// Подключаем все, что надо для OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Прочие необходимые
#include <filesystem>
#include <iostream>
#include "snake.h"
#include "shader/shader.h"
#include "texture/texture.h"

struct Control
{
	// Переменные для камеры
	glm::vec3 camera_pos;
	glm::vec3 camera_front;
	glm::vec3 camera_up;
	// Переменные для управления мышью
	GLfloat last_pos_x;
	GLfloat last_pos_y;
	GLfloat yaw;
	GLfloat pitch;
	// Флаг для устранения рывка мыши в момент запуска
	GLfloat first_cursor_call;
	// Состояние клавиш
	bool keys[1024];
};

class Render
{
private:
	// Окно
	GLFWwindow* window;
	// Размеры окна
	int window_width;
	int window_height;

	// Переменные для нормализации отрисовки кадров
	GLfloat delta_time;
	GLfloat last_frame;
	GLfloat current_frame;

	// Переменные для контроля fps
	int frames_per_sec;
	double current_time;
	double last_time;

	// Пути
	std::string working_directory;
	GLchar* field_v_shader_path;
	GLchar* field_f_shader_path;
	GLchar* snake_v_shader_path;
	GLchar* snake_f_shader_path;
	GLchar* grass_path;
	GLchar* bricks_path;

	// Шейдеры
	Shader field_shader;
	Shader snake_shader;

	// Текстуры
	Texture grass;
	Texture bricks;

	// VBO объекты
	GLuint snake_VBO;
	GLuint grass_VBO;
	// VAO объекты
	GLuint snake_VAO;
	GLuint grass_VAO;
	// EBO объекты
	GLuint snake_EBO;
	GLuint grass_EBO;
public:
	// Управление
	Control control;

	Render();
	// Работа с путем к объекту
	void work_on_path(GLchar** path, std::string input_path);
	// Работа с объектами OpenGL
	void init_object(GLuint* VAO, GLuint* VBO, GLuint* EBO,
					 GLfloat* vertices, GLuint* indices);
	// Очистка всего выделяемого
	void free_all();
	// Обработка нажатий
	void key_processing(Snake* snake);
	// Отрисовка
	void draw(Snake* snake);
	// Инициализация
	int init(void (*cursor_callback)(GLFWwindow*, double, double),
			 void (*key_callback)(GLFWwindow*, int, int, int, int),
			 Snake* snake);
};

#endif