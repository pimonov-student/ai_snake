// Подключаем все, что надо для OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Прочие необходимые
#define STB_IMAGE_IMPLEMENTATION
#include <filesystem>
#include <iostream>
#include "snake.h"
#include "shader/shader.h"
#include "texture/texture.h"
#include "render.h"

Render render;
Snake snake;

// Реакция на движение мыши
void cursor_callback(GLFWwindow* window, double pos_x, double pos_y)
{
	if (render.control.first_cursor_call)
	{
		render.control.first_cursor_call = false;

		render.control.last_pos_x = pos_x;
		render.control.last_pos_y = pos_y;
	}

	// Чувствительность мыши
	GLfloat sensitivity = 0.5f;
	// Изменения в координатах
	GLfloat movement_x = (pos_x - render.control.last_pos_x) * sensitivity;
	GLfloat movement_y = (render.control.last_pos_y - pos_y) * sensitivity; // Оконные координаты возрастают сверху вниз

	render.control.last_pos_x = pos_x;
	render.control.last_pos_y = pos_y;

	render.control.yaw += movement_x;
	render.control.pitch += movement_y;

	// Ограничения на "тангаж", чтоб по Y не было проблем с отрисовкой
	if (render.control.pitch > 89.0f)
		render.control.pitch = 89.0f;
	if (render.control.pitch < -89.0f)
		render.control.pitch = -89.0f;

	// Теперь определяем конечный вектор направления камеры
	glm::vec3 front;
	front.x = cos(glm::radians(render.control.pitch)) * cos(glm::radians(render.control.yaw));
	front.y = sin(glm::radians(render.control.pitch));
	front.z = cos(glm::radians(render.control.pitch)) * sin(glm::radians(render.control.yaw));
	render.control.camera_front = glm::normalize(front);
}

// Реакция на нажатия клавиш
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Обрабатываем нажатую и отпущенную кнопки
	if (action == GLFW_PRESS)
	{
		render.control.keys[key] = true;
	}
	if (action == GLFW_RELEASE)
	{
		render.control.keys[key] = false;
	}

	// Если нажали esc, закрываем окно
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

int main()
{
	render.init(&cursor_callback, &key_callback, &snake);

	return 0;
}