#include "snake.h"

Position& Position::operator=(const Position& other)
{
	this->x = other.x;
	this->y = other.y;
	return *this;
}

bool operator==(const Position& left, const Position& right)
{
	return (left.x == right.x && left.y == right.y) ? true : false;
}

Snake::Snake()
{
	srand(time(NULL));

	direction = 'r';
	size = 16;
	step = 1.0f / size;

	default_pos.x = step / 2;
	default_pos.y = step / 2;

	head_pos = default_pos;

	food_pos.x = -0.5 + (step / 2) + step * (rand() % size);
	food_pos.y = -0.5 + (step / 2) + step * (rand() % size);
}

void Snake::game_cycle()
{
	// Передвигаем сегменты тела
	for (int i = body_pos.size() - 1; i > -1; --i)
	{
		if (!i)
		{
			body_pos[i] = head_pos;
		}
		else
		{
			body_pos[i] = body_pos[i - 1];
		}
	}
	// Если голова наткнулась на еду
	if (head_pos.x == food_pos.x && head_pos.y == food_pos.y)
	{
		while (true)
		{
			bool is_free = true;

			food_pos.x = -0.5 + (step / 2) + step * (rand() % size);
			food_pos.y = -0.5 + (step / 2) + step * (rand() % size);

			for (int i = 0; i < body_pos.size(); ++i)
			{
				if (food_pos == body_pos[i])
				{
					is_free = false;
					break;
				}
			}

			if (is_free)
			{
				break;
			}
		}

		if (body_pos.empty())
		{
			tail_pos = head_pos;
		}
		body_pos.push_back(tail_pos);
	}

	// Направления: r - right, l - left, u - up, d - down
	head_pos.x += (direction == 'r' ? step : (direction == 'l' ? -step : 0));
	head_pos.y += (direction == 'u' ? step : (direction == 'd' ? -step : 0));

	// Столкновение со стенами
	if ((std::abs(float(head_pos.x))) > 0.5 || (std::abs(float(head_pos.y))) > 0.5)
	{
		head_pos = default_pos;
		body_pos.clear();
	}
	// Столкновение с телом
	for (int i = 0; i < body_pos.size(); ++i)
	{
		if (head_pos == body_pos[i])
		{
			head_pos = default_pos;
			body_pos.clear();
		}
	}

	glm::mat4 new_head_model(1.0f);
	glm::mat4 new_food_model(1.0f);

	new_head_model = glm::translate(new_head_model, glm::vec3(head_pos.x, head_pos.y, step / 2));
	new_food_model = glm::translate(new_food_model, glm::vec3(food_pos.x, food_pos.y, step / 2));
	new_head_model = glm::scale(new_head_model, glm::vec3(step));
	new_food_model = glm::scale(new_food_model, glm::vec3(step));

	head_model = new_head_model;
	food_model = new_food_model;
	if (!body_pos.empty())
	{
		tail_pos = body_pos.back();
	}
}

GLchar Snake::get_direction()
{
	return direction;
}
glm::mat4 Snake::get_head_model()
{
	return head_model;
}
glm::mat4 Snake::get_food_model()
{
	return food_model;
}
int Snake::get_body_pos_size()
{
	return body_pos.size();
}
Position Snake::get_body_pos(int id)
{
	return body_pos[id];
}
GLfloat Snake::get_step()
{
	return step;
}

void Snake::set_direction(char new_direction)
{
	direction = new_direction;
}