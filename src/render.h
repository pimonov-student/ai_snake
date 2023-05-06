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
#define STB_IMAGE_IMPLEMENTATION
#include "texture/stb_image.h"

class Render
{
private:
	// ����
	GLFWwindow* window;
	// ������� ����
	int window_width;
	int window_height;
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
	// ���������� ��� ������������ ��������� ������
	GLfloat delta_time;
	GLfloat last_frame;
	GLfloat current_frame;
	// ��������� ������
	bool keys[1024];
public:
	Render();
	// ������� �� �������� ����
	void cursor_callback(GLFWwindow* window, double pos_x, double pos_y);
	// ������� �� ������� ������
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	// ��������� �������
	void key_processing(Snake* snake);
	// ���������
	void draw(GLFWwindow* window,
		GLuint grass_VAO, GLuint snake_VAO,
		Shader* field_shader, Shader* snake_shader,
		Texture* grass, Texture* bricks,
		Snake* snake);
	int init();
};
