#include "texture.h"

Texture::Texture(GLchar* texture_path)
{
	// Загружаем изображение
	unsigned char* img = stbi_load(texture_path, &this->width, &this->height, &this->n_channels, 0);

    // Генерируем текстуру
    glGenTextures(1, &this->texture);
    // Указываем ее тип (привязываем)
    glBindTexture(GL_TEXTURE_2D, this->texture);
    // Генерируем текстуру, то есть передаем наше загруженное изображение в генератор
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    // Генерируем так же mipmap к этой текстуре
    glGenerateMipmap(GL_TEXTURE_2D);

    // Очистим память под изображение (оно нам больше не нужно), а также отвяжем текстуру
    stbi_image_free(img);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() = default;