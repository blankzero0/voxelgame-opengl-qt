#include <cmath>
#include <limits>
#include "raycast.h"
#include "world.h"

std::optional<std::tuple<BlockPosition, BlockDirection, float>> raycast(const World& world, Point from, Vector direction, float distance)
{
	// https://www.cse.yorku.ca/~amana/research/grid.pdf

	BlockPosition block_position{static_cast<int64_t>(std::floor(from.x)), static_cast<int64_t>(std::floor(from.y)), static_cast<int64_t>(std::floor(from.z))};

	const Block* block = world.block_at(block_position);
	if (block && block->is_visible()) {
		return std::tuple{block_position, BlockDirection::NONE, 0};
	}

	int64_t step_x;
	float t_max_x;
	float t_delta_x = 1 / std::abs(direction.x);
	if (direction.x > 0) {
		step_x = 1;
		t_max_x = (static_cast<int64_t>(std::ceil(from.x)) - from.x) / direction.x;
	} else if (direction.x < 0) {
		step_x = -1;
		t_max_x = (block_position.x - from.x) / direction.x;
	} else {
		step_x = 0;
		t_max_x = std::numeric_limits<float>::infinity();
	}

	int64_t step_y;
	float t_max_y;
	float t_delta_y = 1 / std::abs(direction.y);
	if (direction.y > 0) {
		step_y = 1;
		t_max_y = (static_cast<int64_t>(std::ceil(from.y)) - from.y) / direction.y;
	} else if (direction.y < 0) {
		step_y = -1;
		t_max_y = (block_position.y - from.y) / direction.y;
	} else {
		step_y = 0;
		t_max_y = std::numeric_limits<float>::infinity();
	}

	int64_t step_z;
	float t_max_z;
	float t_delta_z = 1 / std::abs(direction.z);
	if (direction.z > 0) {
		step_z = 1;
		t_max_z = (static_cast<int64_t>(std::ceil(from.z)) - from.z) / direction.z;
	} else if (direction.z < 0) {
		step_z = -1;
		t_max_z = (block_position.z - from.z) / direction.z;
	} else {
		step_z = 0;
		t_max_z = std::numeric_limits<float>::infinity();
	}
	
	while (true) {
		if (t_max_x <= t_max_y) {
			if (t_max_x <= t_max_z) {
				if (t_max_x > distance)
					break;
				
				block_position.x += step_x;

				const Block* block = world.block_at(block_position);
				if (block && block->is_visible()) {
					return std::tuple{block_position, step_x == 1 ? BlockDirection::X_NEG : BlockDirection::X_POS, t_max_x};
				}

				t_max_x += t_delta_x;
			} else {
				if (t_max_z > distance)
					break;

				block_position.z += step_z;

				const Block* block = world.block_at(block_position);
				if (block && block->is_visible()) {
					return std::tuple{block_position, step_z == 1 ? BlockDirection::Z_NEG : BlockDirection::Z_POS, t_max_z};
				}

				t_max_z += t_delta_z;
			}
		} else {
			if (t_max_z <= t_max_y) {
				if (t_max_z > distance)
					break;

				block_position.z += step_z;

				const Block* block = world.block_at(block_position);
				if (block && block->is_visible()) {
					return std::tuple{block_position, step_z == 1 ? BlockDirection::Z_NEG : BlockDirection::Z_POS, t_max_z};
				}

				t_max_z += t_delta_z;
			} else {
				if (t_max_y > distance)
					break;

				block_position.y += step_y;

				const Block* block = world.block_at(block_position);
				if (block && block->is_visible()) {
					return std::tuple{block_position, step_y == 1 ? BlockDirection::Y_NEG : BlockDirection::Y_POS, t_max_y};
				}

				t_max_y += t_delta_y;
			}
		}
	}

	return {};
}
