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

	// ����������
	Control control;

	// ����
	std::string working_directory;
	GLchar* vertex_shader_path;
	GLchar* fragment_shader_path;
	GLchar* field_texture_path;
	GLchar* body_texture_path;
	GLchar* food_texture_path;

	// ������
	Shader shader;

	// ��������
	Texture field;
	Texture body;
	Texture food;

	// VBO �������
	GLuint body_VBO;
	GLuint field_VBO;
	// VAO �������
	GLuint body_VAO;
	GLuint field_VAO;
	// EBO �������
	GLuint body_EBO;
	GLuint field_EBO;
public:
	Render();
	// ������ � ����� � �������
	void work_on_path(GLchar** path, std::string input_path);
	// ������ � ��������� OpenGL
	void init_object(GLuint* VAO, GLuint* VBO, GLuint* EBO,
					 GLfloat* vertices, GLuint* indices,
					 int vertices_sizeof, int indices_sizeof);
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
	// get'�
	glm::vec3 get_camera_pos();
	glm::vec3 get_camera_front();
	glm::vec3 get_camera_up();
	GLfloat get_last_pos_x();
	GLfloat get_last_pos_y();
	GLfloat get_yaw();
	GLfloat get_pitch();
	GLfloat get_first_cursor_call();
	bool get_key(int id);
	// set'�
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