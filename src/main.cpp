#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <iostream>
#include "shader/shader.h"
#include "texture/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "texture/stb_image.h"

// Пути к некоторым файлам
GLchar* v_shader_path;
GLchar* f_shader_path;
GLchar* wall_texture_path;
GLchar* shrek_texture_path;

// Переменные для камеры
glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

// Реакция на движение курсора
void cursor_callback(GLFWwindow* window, double x_pos, double y_pos)
{

}

// Состояние клавиш
bool keys[1024];

// Реакция на нажатия
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // Фиксируем нажатия
    if (action == GLFW_PRESS)
    {
        keys[key] = true;
    }
    if (action == GLFW_RELEASE)
    {
        keys[key] = false;
    }
    // Закрываем окно
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

// Движение камеры
void check_keys()
{
    // Управление камерой
    GLfloat camera_step = 0.05f;
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

// Функция подгатавливает расположение файлов необходимых
void work_on_paths(std::string wd)
{
    std::string v_shader = wd + "\\src\\shader\\vertex_shader.vs";
    std::string f_shader = wd + "\\src\\shader\\fragment_shader.fs";
    std::string wall = wd + "\\src\\texture\\img\\wall.jpg";
    std::string shrek = wd + "\\src\\texture\\img\\shrek.png";

    char* v_tmp = v_shader.data();
    char* f_tmp = f_shader.data();
    char* wall_tmp = wall.data();
    char* shrek_tmp = shrek.data();

    v_shader_path = new char[v_shader.length() + 1];
    f_shader_path = new char[f_shader.length() + 1];
    wall_texture_path = new char[wall.length() + 1];
    shrek_texture_path = new char[shrek.length() + 1];

    strcpy(v_shader_path, v_tmp);
    strcpy(f_shader_path, f_tmp);
    strcpy(wall_texture_path, wall_tmp);
    strcpy(shrek_texture_path, shrek_tmp);
}

int main(void)
{
    // Работаем с путями к файлам путем избиения костылями
    std::filesystem::path cwd = std::filesystem::current_path();
    std::string wd = cwd.string();
    wd.resize(wd.length() - 6);
    work_on_paths(wd);

    // Инициализация и настройка GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    // "Установка профайла, для которого создается контекст"
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Запрет на динамическое изменение окна
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


    // Создаем объект окна
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Задаем window рабочим окном
    glfwMakeContextCurrent(window);

    // Указываем glfw, чтоб он фиксировал курсор мыши, но не отображал его
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Передаем рукописную callback-функцию для мыши
    glfwSetCursorPosCallback(window, cursor_callback);
    // Передаем нашу рукописную callback-функцию для клавиатуры
    glfwSetKeyCallback(window, key_callback);


    // Загружаем библиотеку GLAD (OpenGL)
    if (!gladLoadGL())
    {
        std::cout << "Can't load GLAD" << std::endl;
        return -1;
    }


    // Передаем OpenGL размер window
    int w_width, w_height;
    glfwGetFramebufferSize(window, &w_width, &w_height);
    glViewport(0, 0, w_width, w_height);


    // Создаем шейдеры (шейдерную программу)
    Shader shader(v_shader_path, f_shader_path);


    // Координаты (в том числе текстурные) объекта кубик (локальные координаты относительно его центра)
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // Используем один объект кубик и отрисуем таких 10 штук
    // Для этого в основном цикле в model матрицы передадим вот эти вектора
    glm::vec3 cubes_coords[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };


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
    // Сначала укажем позицию, увеличив шаг
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // То же самое для текстуры
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // IV
    glBindVertexArray(0);


    // Устанавливаем флажок, без которого stb_image будет загружать изображения перевернутыми
    stbi_set_flip_vertically_on_load(true);
    // Создаем текстуры
    Texture wall(wall_texture_path);
    Texture shrek(shrek_texture_path);


    // Эта функция устанавливает режим отрисовки
    // Второй аргумент, например: GL_LINE - только линии, GL_FILL - заливка
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Включаем проверку Z-буфера (чтобы корректно работало перекрытие олних фрагментов другими)
    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(window))
    {
        shader.use();
        glfwPollEvents();
        // Движение камеры
        check_keys();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // Помимо очистки цветов изображения, будем очищать Z-буфер от значений предыдущего кадра
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Сделаем активным текстурный блок 0 и закрепим за ним текстуру стены
        glActiveTexture(GL_TEXTURE0);
        // Привязываем текстуру, чтобы переписанный фрагментный шейдер ее обнаружил (в нем мы указали ее как uniform)
        glBindTexture(GL_TEXTURE_2D, wall.texture);
        // Определяем uniform переменную
        glUniform1i(glGetUniformLocation(shader.program, "texture_one"), 0);
        // Делаем то же самое с шреком
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shrek.texture);
        glUniform1i(glGetUniformLocation(shader.program, "texture_two"), 1);




        glBindVertexArray(VAO);

        // Матрицы
        for (GLint i = 0; i < 10; ++i)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubes_coords[i]);
            model = glm::rotate(model, (GLfloat)glfwGetTime() * i, glm::vec3(1.0f, 1.0f, 0.0f));
            GLint model_loc = glGetUniformLocation(shader.program, "model");
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // По сути матрица view есть матрица "вида из камеры", зададим ее через функцию glm::lookAt
        glm::mat4 view(1.0f);
        // Первый аргумент - вектор на камеру, второй - вектор на точку, куда камера смотрит, третий - вспомогательный вектор для создания системы координат
        view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(50.0f), (GLfloat)w_width / (GLfloat)w_height, 0.1f, 300.0f);

        // Определяем наши матрицы как uniform переменные для вершинного шейдера
        GLint view_loc = glGetUniformLocation(shader.program, "view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
        GLint projection_loc = glGetUniformLocation(shader.program, "projection");
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

        // Отвязываем текстуры
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);


        glfwSwapBuffers(window);
    }

    // Очищаем выделенную под эти объекты память
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // После завершения цикла, закрывавем окно
    glfwTerminate();

    return 0;
}