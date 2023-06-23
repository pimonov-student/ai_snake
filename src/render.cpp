#include "render.h"

Render::Render()
{
	window_width = 1200;
	window_height = 900;

	delta_time = 0.0f;
	last_frame = 0.0f;
	current_frame = 0.0f;

	frames_per_sec = 5;
	current_time = 0;
	last_time = 0;

	control.camera_pos = glm::vec3(0.0f, 0.0f, 1.5f);
	control.camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
	control.camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

	control.last_pos_x = 600;
	control.last_pos_y = 450;
	control.yaw = -90.0f;
	control.pitch = 0.0f;

	control.first_cursor_call = true;
}

void Render::work_on_path(GLchar** path, std::string input_path)
{
	input_path = working_directory + input_path;
	char* tmp = input_path.data();
	*path = new char[input_path.length() + 1];
	strcpy(*path, tmp);
}

void Render::init_object(GLuint* VAO, GLuint* VBO, GLuint* EBO,
						 GLfloat* vertices, GLuint* indices)
{
	// Работа с VAO
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	// Работа с VBO
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Работа с EBO
	glGenBuffers(1, EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Render::free_all()
{
	// Очищаем выделенную под объекты память
	glDeleteVertexArrays(1, &grass_VAO);
	glDeleteBuffers(1, &grass_VBO);
	glDeleteBuffers(1, &grass_EBO);
	glDeleteVertexArrays(1, &snake_VAO);
	glDeleteBuffers(1, &snake_VBO);
	glDeleteBuffers(1, &snake_EBO);
}

void Render::key_processing(Snake* snake)
{
	// Управление камерой
	GLfloat camera_step = 0.5f * delta_time;
	if (control.keys[GLFW_KEY_W])
	{
		control.camera_pos += camera_step * control.camera_front;
	}
	if (control.keys[GLFW_KEY_S])
	{
		control.camera_pos -= camera_step * control.camera_front;
	}
	if (control.keys[GLFW_KEY_D])
	{
		control.camera_pos += camera_step * glm::normalize(glm::cross(control.camera_front, control.camera_up));
	}
	if (control.keys[GLFW_KEY_A])
	{
		control.camera_pos -= camera_step * glm::normalize(glm::cross(control.camera_front, control.camera_up));
	}
	if (control.keys[GLFW_KEY_Q])
	{
		control.camera_pos += camera_step * control.camera_up;
	}
	if (control.keys[GLFW_KEY_E])
	{
		control.camera_pos -= camera_step * control.camera_up;
	}

	// Управление змейкой
	if (control.keys[GLFW_KEY_UP] && snake->get_direction() != 'd')
	{
		snake->set_direction('u');
	}
	if (control.keys[GLFW_KEY_DOWN] && snake->get_direction() != 'u')
	{
		snake->set_direction('d');
	}
	if (control.keys[GLFW_KEY_RIGHT] && snake->get_direction() != 'l')
	{
		snake->set_direction('r');
	}
	if (control.keys[GLFW_KEY_LEFT] && snake->get_direction() != 'r')
	{
		snake->set_direction('l');
	}
}

void Render::draw(Snake* snake)
{
	// Заливка фона
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// Очищаем буфер цвета и Z-буфер
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Забиваем текстурами
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grass.texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bricks.texture);


	// Отрисовка поля
	field_shader.use();

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 view(1.0f);
	view = glm::lookAt(control.camera_front,
					   control.camera_pos + control.camera_front,
					   control.camera_up);
	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(50.0f), (GLfloat)window_width / (GLfloat)window_height, 0.1f, 300.0f);

	GLint model_loc = glGetUniformLocation(field_shader.program, "model");
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
	GLint view_loc = glGetUniformLocation(field_shader.program, "view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
	GLint projection_loc = glGetUniformLocation(field_shader.program, "projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(grass_VAO);
	glUniform1i(glGetUniformLocation(field_shader.program, "our_texture"), 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	// Отрисовка змейки
	snake_shader.use();

	glm::mat4 s_model(1.0f);
	s_model = snake->snake_control(s_model);
	s_model = glm::scale(s_model, glm::vec3(0.0625f));

	GLint s_model_loc = glGetUniformLocation(snake_shader.program, "model");
	glUniformMatrix4fv(s_model_loc, 1, GL_FALSE, glm::value_ptr(s_model));
	GLint s_view_loc = glGetUniformLocation(snake_shader.program, "view");
	glUniformMatrix4fv(s_view_loc, 1, GL_FALSE, glm::value_ptr(view));
	GLint s_projection_loc = glGetUniformLocation(snake_shader.program, "projection");
	glUniformMatrix4fv(s_projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(snake_VAO);
	glUniform1i(glGetUniformLocation(snake_shader.program, "our_texture"), 1);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Отвязываем все
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);


	glfwSwapBuffers(window);
}

int Render::init(void (*cursor_callback)(GLFWwindow*, double, double),
				 void (*key_callback)(GLFWwindow*, int, int, int, int),
				 Snake* snake)
{
	// Определение рабочей директории
	std::filesystem::path cwd = std::filesystem::current_path();
	working_directory = cwd.string();
	working_directory.resize(working_directory.length() - 6);
	// Работа с путями
	work_on_path(&field_v_shader_path, "\\src\\shader\\field_v_shader.vs");
	work_on_path(&field_f_shader_path, "\\src\\shader\\field_f_shader.fs");
	work_on_path(&snake_v_shader_path, "\\src\\shader\\snake_v_shader.vs");
	work_on_path(&snake_f_shader_path, "\\src\\shader\\snake_f_shader.fs");
	work_on_path(&grass_path, "\\src\\texture\\img\\grass.jpg");
	work_on_path(&bricks_path, "\\src\\texture\\img\\bricks.jpg");

	// Временные переменные, необходимые для получения размеров окна
	int temp_window_width, temp_window_height;

	// Инициализация и настройка GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Инициализируем объект окна
	window = glfwCreateWindow(window_width, window_height, "AI SNAKE", nullptr, nullptr);
	// Проверка инициализации окна
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
	glfwSetCursorPosCallback(window, *cursor_callback);
	// Передаем нашу callback функцию для клавиатуры
	glfwSetKeyCallback(window, *key_callback);

	// Загружаем GLAD (OpenGL) и проверяем загрузку
	if (!gladLoadGL())
	{
		std::cout << "Can't load GLAD" << std::endl;
		return -1;
	}

	// Передаем OpenGL размер window
	glfwGetFramebufferSize(window, &temp_window_width, &temp_window_height);
	glViewport(0, 0, temp_window_width, temp_window_height);


	// Создаем шейдерные программы
	field_shader = Shader(field_v_shader_path, field_f_shader_path);
	snake_shader = Shader(snake_v_shader_path, snake_f_shader_path);


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
	init_object(&grass_VAO, &grass_VBO, &grass_EBO, grass_vertices, grass_indices);
	// Змея
	init_object(&snake_VAO, &snake_VBO, &snake_EBO, snake_vertices, snake_indices);


	// Флаг для корректной загрузки изображений через stb_image
	stbi_set_flip_vertically_on_load(true);
	// Создаем объекты текстур
	grass = Texture(grass_path);
	bricks = Texture(bricks_path);


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
		// Движение камеры
		key_processing(snake);

		// Отрисовка
		current_time = glfwGetTime();
		if (current_time - last_time >= 1.0 / frames_per_sec)
		{
			last_time = current_time;
			draw(snake);
		}
	}

	// Очищаем память
	free_all();
	// После завершения цикла, закрывая окно
	glfwTerminate();
}