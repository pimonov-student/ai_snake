// Подключаем все, что надо для OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Прочие необходимые
#include <filesystem>
#include <iostream>
#include "shader/shader.h"
#include "texture/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "texture/stb_image.h"

// Пути к нужным файлам
GLchar* v_shader_path;
GLchar* f_shader_path;
// -------- СЮДА ЖЕ ДОБАВИТЬ ПУТИ К ТЕКСТУРАМ --------

// Переменные для камеры
glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

// Переменные для нормализации отрисовки кадров
GLfloat delta_time = 0.0f;
GLfloat last_frame = 0.0f;
GLfloat current_frame = 0.0f;

// Состояние клавиш
bool keys[1024];

// Реакция на нажатия клавиш
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// Обрабатываем нажатую и отпущенную кнопки
	if (action == GLFW_PRESS)
	{
		keys[key] = true;
	}
	if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}

	// Если нажали esc, закрываем окно
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

// Функция подготавливает пути к необходимым файлам
void work_on_paths(std::string wd)
{
	std::string v_shader = wd + "\\src\\shader\\vertex_shader.vs";
	std::string f_shader = wd + "\\src\\shader\\fragment_shader.fs";

	char* v_tmp = v_shader.data();
	char* f_tmp = f_shader.data();

	v_shader_path = new char[v_shader.length() + 1];
	f_shader_path = new char[f_shader.length() + 1];

	strcpy(v_shader_path, v_tmp);
	strcpy(f_shader_path, f_tmp);
}

int main()
{
	// Работа с путями к файлам
	std::filesystem::path cwd = std::filesystem::current_path();
	std::string wd = cwd.string();
	wd.resize(wd.length() - 6);
	work_on_paths(wd);


	// Инициализация и настройка GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Создаем объект окна
	GLFWwindow* window = glfwCreateWindow(800, 600, "AI SNAKE", nullptr, nullptr);
	// Проверка создания окна
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Задаем созданное окно текущим
	glfwMakeContextCurrent(window);

	// Передаем нашу callback функцию для клавиатуры
	glfwSetKeyCallback(window, key_callback);


	// Загружаем GLAD (OpenGL) и проверяем загрузку
	if (!gladLoadGL())
	{
		std::cout << "Can't load GLAD" << std::endl;
		return -1;
	}


	// Передаем OpenGL размер window
	int w_width, w_height;
	glfwGetFramebufferSize(window, &w_width, &w_height);
	glViewport(0, 0, w_width, w_height);


	// Собираем шейдерную программу
	Shader shader(v_shader_path, f_shader_path);


	// -------- СЮДА КООРДИНАТЫ (ТЕКСТУРНЫЕ ТОЖЕ) ОБЪЕКТОВ --------
	GLfloat vertices[] = { 0 };
	// -------- СЮДА ВЕКТОРЫ ДЛЯ РАСПОЛОЖЕНИЯ ОБЪЕКТОВ --------
	glm::vec3 smth_coords[] = { glm::vec3(0, 0, 0) };


	// I
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// II
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// III
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	// IV
	glBindVertexArray(0);


	// Флаг для корректной загрузки изображений через stb_image
	stbi_set_flip_vertically_on_load(true);
	// -------- ТУТ СОЗДАЕМ ЧЕРЕЗ Texture ОБЪЕКТЫ ТЕКСТУР --------


	// Режим отрисовки
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Проверка Z-буфера для корректного наложения объектов друг на друга
	glEnable(GL_DEPTH_TEST);


	// Основной цикл отрисовки
	while (!glfwWindowShouldClose(window))
	{
		// Определяем delta_time
		current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		// Обрабатываем события
		glfwPollEvents();

		// Заливка фона
		glClearColor(0.2f, 0.2f, 0.1f, 1.0f);
		// Очищаем буфер цвета и Z-буфер
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	// Очищаем выделенную под эти объекты память
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// После завершения цикла, закрывая окно
	glfwTerminate();

	return 0;
}