#include "shader.h"

Shader::Shader(const GLchar* vertex_path, const GLchar* fragment_path)
{
	// Исходники шейдеров
	std::string vertex_code, fragment_code;
	// Потоки для чтения из файлов
	std::ifstream vertex_file, fragment_file;
	// Потоки для чтения из прочитанных из файлов строк
	std::stringstream vertex_stream, fragment_stream;
	// Как понял, проверка возможности этих потоков кидать исключенимя
	vertex_file.exceptions(std::ifstream::badbit);
	fragment_file.exceptions(std::ifstream::badbit);

	try
	{
		// Открываем файлы по их пути
		vertex_file.open(vertex_path);
		fragment_file.open(fragment_path);
		// Считываем из файлов в потоки
		vertex_stream << vertex_file.rdbuf();
		fragment_stream << fragment_file.rdbuf();
		// Закрываем файлы
		vertex_file.close();
		fragment_file.close();
		// Преобразовываем потоки в строки
		vertex_code = vertex_stream.str();
		fragment_code = fragment_stream.str();
	}
	catch(std::ifstream::failure error)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	// Переходим к работе с OpenGL
	// Те же исходники, преобразовываем тип
	const GLchar* gl_vertex_code = vertex_code.c_str();
	const GLchar* gl_fragment_code = fragment_code.c_str();
	// По сути все то же, что и в main было, то есть сборка, линковка, проверка всего этого
	GLuint vertex_shader, fragment_shader;

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &gl_vertex_code, NULL);
	glCompileShader(vertex_shader);
	check_shader_compilation(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &gl_fragment_code, NULL);
	glCompileShader(fragment_shader);
	check_shader_compilation(fragment_shader);

	this->program = glCreateProgram();
	glAttachShader(this->program, vertex_shader);
	glAttachShader(this->program, fragment_shader);
	glLinkProgram(this->program);
	check_program_linking(this->program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

Shader::~Shader() = default;

void Shader::check_shader_compilation(GLuint shader)
{
	GLint success;
	GLchar info_log[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
	}
}

void Shader::check_program_linking(GLuint program)
{
	GLint success;
	GLchar info_log[512];

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, info_log);
		std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
	}
}

void Shader::use()
{
	glUseProgram(this->program);
}