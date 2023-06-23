#pragma once

#ifndef RENDER_H
#define RENDER_H

// ���������� ���, ��� ���� ��� OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// ������ �����������
#include <filesystem>
#include <iostream>
#include "snake.h"
#include "shader/shader.h"
#include "texture/texture.h"

struct Control
{
	// ���������� ��� ������
	glm::vec3 camera_pos;
	glm::vec3 camera_front;
	glm::vec3 camera_up;
	// ���������� ��� ���������� �����
	GLfloat last_pos_x;
	GLfloat last_pos_y;
	GLfloat yaw;
	GLfloat pitch;
	// ���� ��� ���������� ����� ���� � ������ �������
	GLfloat first_cursor_call;
	// ��������� ������
	bool keys[1024];
};

class Render
{
private:
	// ����
	GLFWwindow* window;
	// ������� ����
	int window_width;
	int window_height;

	// ���������� ��� ������������ ��������� ������
	GLfloat delta_time;
	GLfloat last_frame;
	GLfloat current_frame;

	// ���������� ��� �������� fps
	int frames_per_sec;
	double current_time;
	double last_time;

	// ����
	std::string working_directory;
	GLchar* field_v_shader_path;
	GLchar* field_f_shader_path;
	GLchar* snake_v_shader_path;
	GLchar* snake_f_shader_path;
	GLchar* grass_path;
	GLchar* bricks_path;

	// �������
	Shader field_shader;
	Shader snake_shader;

	// ��������
	Texture grass;
	Texture bricks;

	// VBO �������
	GLuint snake_VBO;
	GLuint grass_VBO;
	// VAO �������
	GLuint snake_VAO;
	GLuint grass_VAO;
	// EBO �������
	GLuint snake_EBO;
	GLuint grass_EBO;
public:
	// ����������
	Control control;

	Render();
	// ������ � ����� � �������
	void work_on_path(GLchar** path, std::string input_path);
	// ������ � ��������� OpenGL
	void init_object(GLuint* VAO, GLuint* VBO, GLuint* EBO,
					 GLfloat* vertices, GLuint* indices);
	// ������� ����� �����������
	void free_all();
	// ��������� �������
	void key_processing(Snake* snake);
	// ���������
	void draw(Snake* snake);
	// �������������
	int init(void (*cursor_callback)(GLFWwindow*, double, double),
			 void (*key_callback)(GLFWwindow*, int, int, int, int),
			 Snake* snake);
};

#endif