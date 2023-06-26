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
#include <string>
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

	// Управление
	Control control;

	// Пути
	std::string working_directory;
	GLchar* vertex_shader_path;
	GLchar* fragment_shader_path;
	GLchar* field_texture_path;
	GLchar* body_texture_path;
	GLchar* food_texture_path;

	// Шейдер
	Shader shader;

	// Текстуры
	Texture field;
	Texture body;
	Texture food;

	// VBO объекты
	GLuint body_VBO;
	GLuint field_VBO;
	// VAO объекты
	GLuint body_VAO;
	GLuint field_VAO;
	// EBO объекты
	GLuint body_EBO;
	GLuint field_EBO;
public:
	Render();
	// Работа с путем к объекту
	void work_on_path(GLchar** path, std::string input_path);
	// Работа с объектами OpenGL
	void init_object(GLuint* VAO, GLuint* VBO, GLuint* EBO,
					 GLfloat* vertices, GLuint* indices,
					 int vertices_sizeof, int indices_sizeof);
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
	// get'ы
	glm::vec3 get_camera_pos();
	glm::vec3 get_camera_front();
	glm::vec3 get_camera_up();
	GLfloat get_last_pos_x();
	GLfloat get_last_pos_y();
	GLfloat get_yaw();
	GLfloat get_pitch();
	GLfloat get_first_cursor_call();
	bool get_key(int id);
	// set'ы
	void set_camera_pos(glm::vec3 new_value);
	void set_camera_front(glm::vec3 new_value);
	void set_camera_up(glm::vec3 new_value);
	void set_last_pos_x(GLfloat new_value);
	void set_last_pos_y(GLfloat new_value);
	void set_yaw(GLfloat new_value);
	void set_pitch(GLfloat new_value);
	void set_first_cursor_call(GLfloat new_value);
	void set_key(int id, bool new_value);
};

#endif