// Подключаем все, что надо для OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Прочие необходимые
#include <filesystem>
#include <iostream>
#include "render.h"
#include "snake.h"
#include "shader/shader.h"
#include "texture/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "texture/stb_image.h"

// Функция подготавливает пути к необходимым файлам
void work_on_paths(std::string wd)
{
	std::string field_v_shader = wd + "\\src\\shader\\field_v_shader.vs";
	std::string field_f_shader = wd + "\\src\\shader\\field_f_shader.fs";
	std::string snake_v_shader = wd + "\\src\\shader\\snake_v_shader.vs";
	std::string snake_f_shader = wd + "\\src\\shader\\snake_f_shader.fs";
	std::string grass = wd + "\\src\\texture\\img\\grass.jpg";
	std::string bricks = wd + "\\src\\texture\\img\\bricks.jpg";

	char* field_v_tmp = field_v_shader.data();
	char* field_f_tmp = field_f_shader.data();
	char* snake_v_tmp = snake_v_shader.data();
	char* snake_f_tmp = snake_f_shader.data();
	char* grass_tmp = grass.data();
	char* bricks_tmp = bricks.data();

	field_v_shader_path = new char[field_v_shader.length() + 1];
	field_f_shader_path = new char[field_f_shader.length() + 1];
	snake_v_shader_path = new char[snake_v_shader.length() + 1];
	snake_f_shader_path = new char[snake_f_shader.length() + 1];
	grass_path = new char[grass.length() + 1];
	bricks_path = new char[bricks.length() + 1];

	strcpy(field_v_shader_path, field_v_tmp);
	strcpy(field_f_shader_path, field_f_tmp);
	strcpy(snake_v_shader_path, snake_v_tmp);
	strcpy(snake_f_shader_path, snake_f_tmp);
	strcpy(grass_path, grass_tmp);
	strcpy(bricks_path, bricks_tmp);
}

int main()
{
	// Создаем объекты Render и Snake
	Render render;
	Snake snake;


	// Работа с путями к файлам
	std::filesystem::path cwd = std::filesystem::current_path();
	std::string wd = cwd.string();
	wd.resize(wd.length() - 6);
	work_on_paths(wd);


	// Передаем OpenGL размер window
	glfwGetFramebufferSize(window, &w_width, &w_height);
	glViewport(0, 0, w_width, w_height);


	// Собираем шейдерные программы
	Shader field_shader(field_v_shader_path, field_f_shader_path);
	Shader snake_shader(snake_v_shader_path, snake_f_shader_path);


	// Вершины
	GLfloat grass_vertices[] =
	{	
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
		-0.5f, 0.5f,  0.0f,		0.0f, 1.0f,
		0.5f,  -0.5f, 0.0f,		1.0f, 0.0f,
		0.5f,  0.5f,  0.0f,		1.0f, 1.0f
	};
	GLfloat snake_vertices[] =
	{
		// Верхняя стенка
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,
		0.5f,  0.5f, 0.5f,		1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		1.0f, 1.0f,
		// Нижняя стенка
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
		0.5f,  -0.5f, 0.5f,		1.0f, 0.0f,
		0.5f,  -0.5f, -0.5f,	1.0f, 1.0f,
		// Левая стенка
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,
		-0.5f, 0.5f,  -0.5f,	0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f,
		-0.5f, 0.5f,  0.5f,		1.0f, 1.0f,
		// Правая стенка
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f,
		0.5f, 0.5f,  0.5f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f,
		0.5f, 0.5f,  -0.5f,		1.0f, 1.0f,
		// Задняя стенка
		0.5f,  -0.5f, -0.5f,	0.0f, 0.0f,
		0.5f,  0.5f,  -0.5f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f,
		-0.5f, 0.5f,  -0.5f,	1.0f, 1.0f,
		// Передняя стенка
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,
		0.5f,  -0.5f, 0.5f,		1.0f, 0.0f,
		0.5f,  0.5f, 0.5f,		1.0f, 1.0f
	};
	// Индексы
	GLuint grass_indices[] =
	{
		0, 1, 2,
		1, 2, 3
	};
	GLuint snake_indices[] =
	{
		0, 1, 2,
		1, 2, 3,
		4, 5, 6,
		5, 6, 7,
		8, 9, 10,
		9, 10, 11,
		12, 13, 14,
		13, 14, 15,
		16, 17, 18,
		17, 18, 19,
		20, 21, 22,
		21, 22, 23
	};


	// Трава
	GLuint grass_VAO;
	glGenVertexArrays(1, &grass_VAO);
	glBindVertexArray(grass_VAO);

	GLuint grass_VBO;
	glGenBuffers(1, &grass_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, grass_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grass_vertices), grass_vertices, GL_STATIC_DRAW);

	GLuint grass_EBO;
	glGenBuffers(1, &grass_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grass_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grass_indices), grass_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Змея (одна секция)
	GLuint snake_VAO;
	glGenVertexArrays(1, &snake_VAO);
	glBindVertexArray(snake_VAO);

	GLuint snake_VBO;
	glGenBuffers(1, &snake_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, snake_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(snake_vertices), snake_vertices, GL_STATIC_DRAW);

	GLuint snake_EBO;
	glGenBuffers(1, &snake_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, snake_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(snake_indices), snake_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Флаг для корректной загрузки изображений через stb_image
	stbi_set_flip_vertically_on_load(true);
	// Создаем объекты текстур
	Texture grass(grass_path);
	Texture bricks(bricks_path);


	// Режим отрисовки
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Проверка Z-буфера для корректного наложения объектов друг на друга
	glEnable(GL_DEPTH_TEST);

	int frames_per_sec = 5;
	double current_time = 0;
	double last_time = 0;

	// Основной цикл отрисовки
	while (!glfwWindowShouldClose(window))
	{
		// Определяем delta_time
		current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		// Обрабатываем события
		glfwPollEvents();
		// Движение камеры
		key_processing();

		// Отрисовка
		current_time = glfwGetTime();
		if (current_time - last_time >= 1.0 / frames_per_sec)
		{
			last_time = current_time;
			draw(window, grass_VAO, snake_VAO, &field_shader, &snake_shader, &grass, &bricks);
		}
	}

	// Очищаем выделенную под эти объекты память
	glDeleteVertexArrays(1, &grass_VAO);
	glDeleteBuffers(1, &grass_VBO);
	// После завершения цикла, закрывая окно
	glfwTerminate();

	return 0;
}