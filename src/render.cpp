#include "render.h"

//class Render
//{
//private:
//	// Окно
//	GLFWwindow* window;
//	// Размеры окна
//	int window_width;
//	int window_height;
//	// Переменные для камеры
//	glm::vec3 camera_pos;
//	glm::vec3 camera_front;
//	glm::vec3 camera_up;
//	// Переменные для управления мышью
//	GLfloat last_pos_x;
//	GLfloat last_pos_y;
//	GLfloat yaw;
//	GLfloat pitch;
//	// Флаг для устранения рывка мыши в момент запуска
//	GLfloat first_cursor_call;
//	// Переменные для нормализации отрисовки кадров
//	GLfloat delta_time;
//	GLfloat last_frame;
//	GLfloat current_frame;
//	// Состояние клавиш
//	bool keys[1024];
//public:
//	Render()
//	{
//		window_width = 1200;
//		window_height = 900;
//		
//		camera_pos = glm::vec3(0.0f, 0.0f, 1.5f);
//		camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
//		camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
//		
//		last_pos_x = 600;
//		last_pos_y = 450;
//		yaw = -90.0f;
//		pitch = 0.0f;
//		
//		first_cursor_call = true;
//		
//		delta_time = 0.0f;
//		last_frame = 0.0f;
//		current_frame = 0.0f;
//	}
//	// Реакция на движение мыши
//	static void cursor_callback(GLFWwindow* window, double pos_x, double pos_y)
//	{
//		if (first_cursor_call)
//		{
//			first_cursor_call = false;
//
//			last_pos_x = pos_x;
//			last_pos_y = pos_y;
//		}
//
//		// Чувствительность мыши
//		GLfloat sensitivity = 0.5f;
//		// Изменения в координатах
//		GLfloat movement_x = (pos_x - last_pos_x) * sensitivity;
//		GLfloat movement_y = (last_pos_y - pos_y) * sensitivity; // Оконные координаты возрастают сверху вниз
//
//		last_pos_x = pos_x;
//		last_pos_y = pos_y;
//
//		yaw += movement_x;
//		pitch += movement_y;
//
//		// Ограничения на "тангаж", чтоб по Y не было проблем с отрисовкой
//		if (pitch > 89.0f)
//			pitch = 89.0f;
//		if (pitch < -89.0f)
//			pitch = -89.0f;
//
//		// Теперь определяем конечный вектор направления камеры
//		glm::vec3 front;
//		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
//		front.y = sin(glm::radians(pitch));
//		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
//		camera_front = glm::normalize(front);
//	}
//	// Реакция на нажатия клавиш
//	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
//	{
//		// Обрабатываем нажатую и отпущенную кнопки
//		if (action == GLFW_PRESS)
//		{
//			keys[key] = true;
//		}
//		if (action == GLFW_RELEASE)
//		{
//			keys[key] = false;
//		}
//
//		// Если нажали esc, закрываем окно
//		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		{
//			glfwSetWindowShouldClose(window, GL_TRUE);
//		}
//	}
//	// Обработка нажатий
//	void key_processing(Snake* snake)
//	{
//		// Управление камерой
//		GLfloat camera_step = 0.5f * delta_time;
//		if (keys[GLFW_KEY_W])
//		{
//			camera_pos += camera_step * camera_front;
//		}
//		if (keys[GLFW_KEY_S])
//		{
//			camera_pos -= camera_step * camera_front;
//		}
//		if (keys[GLFW_KEY_D])
//		{
//			camera_pos += camera_step * glm::normalize(glm::cross(camera_front, camera_up));
//		}
//		if (keys[GLFW_KEY_A])
//		{
//			camera_pos -= camera_step * glm::normalize(glm::cross(camera_front, camera_up));
//		}
//		if (keys[GLFW_KEY_Q])
//		{
//			camera_pos += camera_step * camera_up;
//		}
//		if (keys[GLFW_KEY_E])
//		{
//			camera_pos -= camera_step * camera_up;
//		}
//
//		// Управление змейкой
//		if (keys[GLFW_KEY_UP] && snake->get_direction() != 'd')
//		{
//			snake->set_direction('u');
//		}
//		if (keys[GLFW_KEY_DOWN] && snake->get_direction() != 'u')
//		{
//			snake->set_direction('d');
//		}
//		if (keys[GLFW_KEY_RIGHT] && snake->get_direction() != 'l')
//		{
//			snake->set_direction('r');
//		}
//		if (keys[GLFW_KEY_LEFT] && snake->get_direction() != 'r')
//		{
//			snake->set_direction('l');
//		}
//	}
//	// Отрисовка
//	void draw(GLFWwindow* window,
//		GLuint grass_VAO, GLuint snake_VAO,
//		Shader* field_shader, Shader* snake_shader,
//		Texture* grass, Texture* bricks,
//		Snake* snake)
//	{
//		// Заливка фона
//		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//		// Очищаем буфер цвета и Z-буфер
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//
//		// Забиваем текстурами
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, grass->texture);
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(GL_TEXTURE_2D, bricks->texture);
//
//
//		// Отрисовка поля
//		field_shader->use();
//
//		glm::mat4 model(1.0f);
//		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
//		glm::mat4 view(1.0f);
//		view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
//		glm::mat4 projection(1.0f);
//		projection = glm::perspective(glm::radians(50.0f), (GLfloat)window_width / (GLfloat)window_height, 0.1f, 300.0f);
//
//		GLint model_loc = glGetUniformLocation(field_shader->program, "model");
//		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
//		GLint view_loc = glGetUniformLocation(field_shader->program, "view");
//		glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
//		GLint projection_loc = glGetUniformLocation(field_shader->program, "projection");
//		glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
//
//		glBindVertexArray(grass_VAO);
//		glUniform1i(glGetUniformLocation(field_shader->program, "our_texture"), 0);
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//
//
//		// Отрисовка змейки
//		snake_shader->use();
//
//		glm::mat4 s_model(1.0f);
//		s_model = snake->snake_control(s_model);
//		s_model = glm::scale(s_model, glm::vec3(0.0625f));
//
//		GLint s_model_loc = glGetUniformLocation(snake_shader->program, "model");
//		glUniformMatrix4fv(s_model_loc, 1, GL_FALSE, glm::value_ptr(s_model));
//		GLint s_view_loc = glGetUniformLocation(snake_shader->program, "view");
//		glUniformMatrix4fv(s_view_loc, 1, GL_FALSE, glm::value_ptr(view));
//		GLint s_projection_loc = glGetUniformLocation(snake_shader->program, "projection");
//		glUniformMatrix4fv(s_projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
//
//		glBindVertexArray(snake_VAO);
//		glUniform1i(glGetUniformLocation(snake_shader->program, "our_texture"), 1);
//		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//
//		// Отвязываем все
//		glBindTexture(GL_TEXTURE_2D, 0);
//		glBindVertexArray(0);
//
//
//		glfwSwapBuffers(window);
//	}
//	int init()
//	{
//		// Инициализация и настройка GLFW
//		glfwInit();
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
//		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//
//		window = glfwCreateWindow(1200, 900, "AI SNAKE", nullptr, nullptr);
//		// Проверка создания окна
//		if (window == nullptr)
//		{
//			std::cout << "Failed to create GLFW window" << std::endl;
//			glfwTerminate();
//			return -1;
//		}
//		// Задаем созданное окно текущим
//		glfwMakeContextCurrent(window);
//
//		// Задаем в glfw фиксацию курсора, но без его отрисовки
//		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//		// Передаем нашу рукописную callback-функцию для мыши
//		glfwSetCursorPosCallback(window, cursor_callback);
//		// Передаем нашу callback функцию для клавиатуры
//		glfwSetKeyCallback(window, key_callback);
//
//		// Загружаем GLAD (OpenGL) и проверяем загрузку
//		if (!gladLoadGL())
//		{
//			std::cout << "Can't load GLAD" << std::endl;
//			return -1;
//		}
//
//		// Передаем OpenGL размер window
//		glfwGetFramebufferSize(window, &window_width, &window_height);
//		glViewport(0, 0, window_width, window_height);
//	}
//};

Render::Render()
{
	window_width = 1200;
	window_height = 900;
			
	camera_pos = glm::vec3(0.0f, 0.0f, 1.5f);
	camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
	camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
			
	last_pos_x = 600;
	last_pos_y = 450;
	yaw = -90.0f;
	pitch = 0.0f;
			
	first_cursor_call = true;
			
	delta_time = 0.0f;
	last_frame = 0.0f;
	current_frame = 0.0f;
}

void Render::cursor_callback(GLFWwindow* window, double pos_x, double pos_y)
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

void Render::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
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