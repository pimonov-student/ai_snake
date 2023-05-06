#include "snake.h"

Snake::Snake()
{
	direction = 'r';
	step = 0.0625f;
}

glm::mat4 Snake::snake_control(glm::mat4 model)
{
	// Направления: r - right, l - left, u - up, d - down
	pos.x += (direction == 'r' ? step : (direction == 'l' ? -step : 0));
	pos.y += (direction == 'u' ? step : (direction == 'd' ? -step : 0));

	if ((std::abs(float(pos.x))) > 0.5)
	{
		pos.x += pos.x > 0 ? -0.5 : 0.5;
	}
	if ((std::abs(float(pos.y))) > 0.5)
	{
		pos.y += pos.y > 0 ? -0.5 : 0.5;
	}

	model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.031251f));
	return model;
}

GLchar Snake::get_direction()
{
	return direction;
}

void Snake::set_direction(char new_direction)
{
	direction = new_direction;
}