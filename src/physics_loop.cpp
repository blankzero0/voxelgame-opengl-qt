#include <functional>
#include "physics_loop.h"
#include "raycast.h"

PhysicsLoop::PhysicsLoop(Player& player, const Input& input, World& world)
		: player(player), input(input), thread(std::bind_front(&PhysicsLoop::run, this)), world(world)
{
}

void PhysicsLoop::run(const std::stop_token& stop_token)
{
	constexpr float dt = 0.01;
	while (!stop_token.stop_requested()) {
		tick(dt);
		std::this_thread::sleep_for(std::chrono::duration<float>(dt));
	}
}

void PhysicsLoop::tick(float delta_time)
{
	place_cooldown = std::max(0.0f, place_cooldown - delta_time);
	destroy_cooldown = std::max(0.0f, destroy_cooldown - delta_time);

	if (input.is_destroy() || input.is_place()) {
		const Point& player_position = player.get_position();
		Point eye_position{player_position.x, player_position.y + Player::eye_height, player_position.z};
		Vector look_direction = rotation_to_forward_vector(player.get_yaw(), player.get_pitch());
		auto looking_at = raycast(world, eye_position, look_direction, 10);

		if (looking_at) {
			if (input.is_place() && place_cooldown == 0) {
				auto [position, direction, _] = *looking_at;

				switch (direction) {
				case X_POS:
					position.x++;
					break;
				case X_NEG:
					position.x--;
					break;
				case Z_POS:
					position.z++;
					break;
				case Z_NEG:
					position.z--;
					break;
				case Y_POS:
					position.y++;
					break;
				case Y_NEG:
					position.y--;
					break;
				case NONE:
					break;
				}

				world.set_block(position, player_placed_block);
				place_cooldown = 0.2;
			} else if (input.is_destroy() && destroy_cooldown == 0) {
				world.set_block(std::get<0>(*looking_at), player_unplaced_block);
				destroy_cooldown = 0.2;
			}
		}
	}

	float movement_speed = 5.0;
	if (input.is_boost())
		movement_speed *= 10;

	Vector forward = rotation_to_forward_vector(player.get_yaw(), player.get_pitch());
	Vector right = rotation_to_right_vector(player.get_yaw(), player.get_pitch());
	constexpr Vector up{0, 1, 0};

	Vector move_dir{};

	if (input.is_forward())
		move_dir += forward;
	if (input.is_backward())
		move_dir -= forward;
	if (input.is_right())
		move_dir += right;
	if (input.is_left())
		move_dir -= right;
	if (input.is_up())
		move_dir += up;
	if (input.is_down())
		move_dir -= up;

	player.set_position(player.get_position() + delta_time * movement_speed * move_dir);
}
