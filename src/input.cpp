#include <Qt>
#include <unordered_map>
#include "input.h"

bool Input::is_forward() const
{
	return forward;
}

void Input::set_forward(bool forward)
{
	Input::forward = forward;
}

bool Input::is_backward() const
{
	return backward;
}

void Input::set_backward(bool backward)
{
	Input::backward = backward;
}

bool Input::is_right() const
{
	return right;
}

void Input::set_right(bool right)
{
	Input::right = right;
}

bool Input::is_left() const
{
	return left;
}

void Input::set_left(bool left)
{
	Input::left = left;
}

bool Input::is_up() const
{
	return up;
}

void Input::set_up(bool up)
{
	Input::up = up;
}

bool Input::is_down() const
{
	return down;
}

void Input::set_down(bool down)
{
	Input::down = down;
}

std::optional<Input::setter> Input::get_setter_from_keycode(int key)
{
	static std::unordered_map<int, Input::setter> key_map{
			{Qt::Key_W, &Input::set_forward},
			{Qt::Key_S, &Input::set_backward},
			{Qt::Key_D, &Input::set_right},
			{Qt::Key_A, &Input::set_left},
			{Qt::Key_Space, &Input::set_up},
			{Qt::Key_Control, &Input::set_down},
			{Qt::Key_Shift, &Input::set_boost},
	};

	const auto& it = key_map.find(key);
	if (it == key_map.end())
		return {};

	return it->second;
}

bool Input::is_boost() const
{
	return boost;
}

void Input::set_boost(bool boost)
{
	Input::boost = boost;
}

bool Input::is_destroy() const
{
	return destroy;
}

void Input::set_destroy(bool destroy)
{
	Input::destroy = destroy;
}

bool Input::is_place() const
{
	return place;
}

void Input::set_place(bool place)
{
	Input::place = place;
}
