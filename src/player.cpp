#include <numbers>
#include <algorithm>
#include "player.h"

void Player::turn(float delta_yaw, float delta_pitch)
{
	yaw += delta_yaw;
	pitch = std::clamp(pitch + delta_pitch, -std::numbers::pi_v<float> / 2, std::numbers::pi_v<float> / 2);
}

float Player::yaw_degrees() const
{
	return yaw / std::numbers::pi_v<float> * 180;
}

float Player::pitch_degrees() const
{
	return pitch / std::numbers::pi_v<float> * 180;
}

Player::Player(const Point& position)
		: position(position), yaw(0), pitch(0), velocity{0, 0, 0}
{}

void Player::add_position_listener(std::function<void(const Point&)>&& listener)
{
	position_listeners.push_back(std::move(listener));
}
