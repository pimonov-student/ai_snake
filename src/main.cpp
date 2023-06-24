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
	if (render.get_first_cursor_call())
	{
		render.set_first_cursor_call(false);

		render.set_last_pos_x(pos_x);
		render.set_last_pos_y(pos_y);
	}

	// Чувствительность мыши
	GLfloat sensitivity = 0.5f;
	// Изменения в координатах
	GLfloat movement_x = (pos_x - render.get_last_pos_x()) * sensitivity;
	GLfloat movement_y = (render.get_last_pos_y() - pos_y) * sensitivity; // Оконные координаты возрастают сверху вниз

	render.set_last_pos_x(pos_x);
	render.set_last_pos_y(pos_y);

	render.set_yaw(render.get_yaw() + movement_x);
	render.set_pitch(render.get_pitch() + movement_y);

	// Ограничения на "тангаж", чтоб по Y не было проблем с отрисовкой
	if (render.get_pitch() > 89.0f)
		render.set_pitch(89.0f);
	if (render.get_pitch() < -89.0f)
		render.set_pitch(- 89.0f);

	// Теперь определяем конечный вектор направления камеры
	glm::vec3 front;
	front.x = cos(glm::radians(render.get_pitch())) * cos(glm::radians(render.get_yaw()));
	front.y = sin(glm::radians(render.get_pitch()));
	front.z = cos(glm::radians(render.get_pitch())) * sin(glm::radians(render.get_yaw()));
	render.set_camera_front(glm::normalize(front));
}

// Реакция на нажатия клавиш
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Обрабатываем нажатую и отпущенную кнопки
	if (action == GLFW_PRESS)
	{
		render.set_key(key, true);
	}
	if (action == GLFW_RELEASE)
	{
		render.set_key(key, false);
	}

	// Если нажали esc, закрываем окно
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

int main()
{
	render.init(cursor_callback, key_callback, &snake);

	return 0;
}