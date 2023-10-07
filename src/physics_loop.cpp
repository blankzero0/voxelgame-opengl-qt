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

	Vector forward = rotation_to_forward_vector(player.get_yaw(), 0);
	Vector right = rotation_to_right_vector(player.get_yaw(), 0);
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

	Point player_position = player.get_position();

	constexpr Vector graivitational_acceleration{0, -9.81, 0};
	Vector player_mechanical_velocity = player.get_velocity() + delta_time * graivitational_acceleration;

	constexpr float ground_contact_distance = 0.125;
	constexpr float jump_velocity = 5;
	if (input.is_up() && player_mechanical_velocity.y < 0 && raycast(world, player_position, -up, ground_contact_distance))
		player_mechanical_velocity.y = jump_velocity;


	Vector player_controlled_velocity = player_mechanical_velocity + movement_speed * move_dir;

	std::array corners = {
		Point{player_position.x - Player::width / 2, player_position.y, player_position.z - Player::width / 2},
		Point{player_position.x + Player::width / 2, player_position.y, player_position.z - Player::width / 2},
		Point{player_position.x - Player::width / 2, player_position.y, player_position.z + Player::width / 2},
		Point{player_position.x + Player::width / 2, player_position.y, player_position.z + Player::width / 2},
		Point{player_position.x - Player::width / 2, player_position.y + Player::height / 2, player_position.z - Player::width / 2},
		Point{player_position.x + Player::width / 2, player_position.y + Player::height / 2, player_position.z - Player::width / 2},
		Point{player_position.x - Player::width / 2, player_position.y + Player::height / 2, player_position.z + Player::width / 2},
		Point{player_position.x + Player::width / 2, player_position.y + Player::height / 2, player_position.z + Player::width / 2},
		Point{player_position.x - Player::width / 2, player_position.y + Player::height, player_position.z - Player::width / 2},
		Point{player_position.x + Player::width / 2, player_position.y + Player::height, player_position.z - Player::width / 2},
		Point{player_position.x - Player::width / 2, player_position.y + Player::height, player_position.z + Player::width / 2},
		Point{player_position.x + Player::width / 2, player_position.y + Player::height, player_position.z + Player::width / 2},
	};

	Vector player_position_delta = delta_time * player_controlled_velocity;

	while (true) {
		bool collides = false;
		float collision_distance = 1;
		BlockDirection collision_direction;
		for (const Point& corner : corners) {
			auto collision = raycast(world, corner, player_position_delta, collision_distance);
			if (collision) {
				collides = true;
				auto [_, direction, distance] = *collision;
				collision_direction = direction;
				collision_distance = distance;
			}
		}

		if (!collides) {
			break;
		}

		switch (collision_direction)
		{
		case BlockDirection::X_NEG:
		case BlockDirection::X_POS:
			player_position_delta.x *= collision_distance;
			player_mechanical_velocity.x = 0;
			break;
		case BlockDirection::Y_NEG:
		case BlockDirection::Y_POS:
			player_position_delta.y *= collision_distance;
			player_mechanical_velocity.y = 0;
			break;
		case BlockDirection::Z_NEG:
		case BlockDirection::Z_POS:
			player_position_delta.z *= collision_distance;
			player_mechanical_velocity.z = 0;
			break;
		case BlockDirection::NONE:
			player_mechanical_velocity = Vector{0, 0, 0};
			break;
		}

		if (collision_direction == BlockDirection::NONE) {
			break;
		}
	}

	player.set_position(player_position + player_position_delta);
	player.set_velocity(player_mechanical_velocity);
}
