#ifndef VOXELGAME_OPENGL_QT_PLAYER_H
#define VOXELGAME_OPENGL_QT_PLAYER_H


#include "geometry.h"

class Player {
public:
	static constexpr float eye_height = 1.7;
	static constexpr float height = 1.8;
	static constexpr float width = 0.5;

	explicit Player(const Point& position);
	void turn(float delta_yaw, float delta_pitch);

	[[nodiscard]] float yaw_degrees() const;
	[[nodiscard]] float pitch_degrees() const;

	[[nodiscard]] float get_yaw() const
	{
		return yaw;
	}

	[[nodiscard]] float get_pitch() const
	{
		return pitch;
	}

	[[nodiscard]] const Point& get_position() const
	{
		return position;
	}

	void set_position(const Point& position)
	{
		Player::position = position;

		for (auto& listener : position_listeners) {
			listener(position);
		}
	}

	void add_position_listener(std::function<void(const Point&)>&& listener);

	[[nodiscard]] const Vector& get_velocity() const
	{
		return velocity;
	}

	void set_velocity(const Vector& velocity)
	{
		Player::velocity = velocity;
	}

private:
	Point position;
	float yaw;
	float pitch;
	Vector velocity;

	std::vector<std::function<void(const Point&)>> position_listeners;
};


#endif //VOXELGAME_OPENGL_QT_PLAYER_H
