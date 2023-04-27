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

// Размеры окна
int w_width, w_height;

// Пути к нужным файлам
GLchar* field_v_shader_path;
GLchar* field_f_shader_path;
GLchar* snake_v_shader_path;
GLchar* snake_f_shader_path;
GLchar* grass_path;
GLchar* bricks_path;

// Переменные для камеры
glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 1.5f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

// Переменные для управления мышью
GLfloat last_pos_x = 600;
GLfloat last_pos_y = 450;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
// Флаг для устранения рывка мыши в момент запуска
GLfloat first_cursor_call = true;

// Переменные для нормализации отрисовки кадров
GLfloat delta_time = 0.0f;
GLfloat last_frame = 0.0f;
GLfloat current_frame = 0.0f;

// Состояние клавиш
bool keys[1024];

// Переменные для движения головы змейки
GLfloat step = 0.0625f;
GLfloat pos = -0.46875f;

// Реакция на движение мыши
void cursor_callback(GLFWwindow* window, double pos_x, double pos_y)
{
	if (first_cursor_call)
	{
		first_cursor_call = false;

		last_pos_x = pos_x;
		last_pos_y = pos_y;
	}

	// Чувствительность мыши
	GLfloat sensitivity = 0.5f;
	// Изменения в координатах
	GLfloat movement_x = (pos_x - last_pos_x) * sensitivity;
	GLfloat movement_y = (last_pos_y - pos_y) * sensitivity; // Оконные координаты возрастают сверху вниз

	last_pos_x = pos_x;
	last_pos_y = pos_y;

	yaw += movement_x;
	pitch += movement_y;

	// Ограничения на "тангаж", чтоб по Y не было проблем с отрисовкой
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// Теперь определяем конечный вектор направления камеры
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	camera_front = glm::normalize(front);
}

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

// Движение камеры
void do_movement()
{
	// Управление камерой
	GLfloat camera_step = 0.5f * delta_time;
	if (keys[GLFW_KEY_W])
	{
		camera_pos += camera_step * camera_front;
	}
	if (keys[GLFW_KEY_S])
	{
		camera_pos -= camera_step * camera_front;
	}
	if (keys[GLFW_KEY_D])
	{
		camera_pos += camera_step * glm::normalize(glm::cross(camera_front, camera_up));
	}
	if (keys[GLFW_KEY_A])
	{
		camera_pos -= camera_step * glm::normalize(glm::cross(camera_front, camera_up));
	}
	if (keys[GLFW_KEY_Q])
	{
		camera_pos += camera_step * camera_up;
	}
	if (keys[GLFW_KEY_E])
	{
		camera_pos -= camera_step * camera_up;
	}
}

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

void render(GLFWwindow* window,
			GLuint grass_VAO, GLuint snake_VAO, 
			Shader* field_shader, Shader* snake_shader, 
			Texture* grass, Texture* bricks)
{
	// Заливка фона
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// Очищаем буфер цвета и Z-буфер
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Забиваем текстурами
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grass->texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bricks->texture);


	// Отрисовка поля
	field_shader->use();

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 view(1.0f);
	view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(50.0f), (GLfloat)w_width / (GLfloat)w_height, 0.1f, 300.0f);

	GLint model_loc = glGetUniformLocation(field_shader->program, "model");
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
	GLint view_loc = glGetUniformLocation(field_shader->program, "view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
	GLint projection_loc = glGetUniformLocation(field_shader->program, "projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(grass_VAO);
	glUniform1i(glGetUniformLocation(field_shader->program, "our_texture"), 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	// Отрисовка змейки
	snake_shader->use();

	glm::mat4 s_model(1.0f);
	s_model = glm::translate(s_model, glm::vec3(pos, 0.03125f, 0.031251f));
	s_model = glm::scale(s_model, glm::vec3(0.0625f));

	GLint s_model_loc = glGetUniformLocation(snake_shader->program, "model");
	glUniformMatrix4fv(s_model_loc, 1, GL_FALSE, glm::value_ptr(s_model));
	GLint s_view_loc = glGetUniformLocation(snake_shader->program, "view");
	glUniformMatrix4fv(s_view_loc, 1, GL_FALSE, glm::value_ptr(view));
	GLint s_projection_loc = glGetUniformLocation(snake_shader->program, "projection");
	glUniformMatrix4fv(s_projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(snake_VAO);
	glUniform1i(glGetUniformLocation(snake_shader->program, "our_texture"), 1);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Отвязываем все
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	pos += step;
	if (pos > 0.5f) pos = -0.46875;

	glfwSwapBuffers(window);
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
	GLFWwindow* window = glfwCreateWindow(1200, 900, "AI SNAKE", nullptr, nullptr);
	// Проверка создания окна
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Задаем созданное окно текущим
	glfwMakeContextCurrent(window);

	// Задаем в glfw фиксацию курсора, но без его отрисовки
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Передаем нашу рукописную callback-функцию для мыши
	glfwSetCursorPosCallback(window, cursor_callback);
	// Передаем нашу callback функцию для клавиатуры
	glfwSetKeyCallback(window, key_callback);


	// Загружаем GLAD (OpenGL) и проверяем загрузку
	if (!gladLoadGL())
	{
		std::cout << "Can't load GLAD" << std::endl;
		return -1;
	}


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

	int frames_per_sec = 30;
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
		do_movement();

		// Отрисовка
		current_time = glfwGetTime();
		if (current_time - last_time >= 1.0 / frames_per_sec)
		{
			last_time = current_time;
			render(window, grass_VAO, snake_VAO, &field_shader, &snake_shader, &grass, &bricks);
		}
	}

	// Очищаем выделенную под эти объекты память
	glDeleteVertexArrays(1, &grass_VAO);
	glDeleteBuffers(1, &grass_VBO);
	// После завершения цикла, закрывая окно
	glfwTerminate();

	return 0;
}