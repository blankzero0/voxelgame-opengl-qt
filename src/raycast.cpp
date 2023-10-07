#include <cmath>
#include <limits>
#include "raycast.h"
#include "world.h"

std::optional<std::tuple<BlockPosition, BlockDirection, float>> raycast(const World& world, Point from, Vector direction, float distance)
{
	// https://www.cse.yorku.ca/~amana/research/grid.pdf

	if (direction.x == 0 && direction.y == 0 && direction.z == 0)
		return {};

	float f_x = std::floor(from.x);
	float f_y = std::floor(from.y);
	float f_z = std::floor(from.z);

	if (
		(direction.x == 0 && from.x == f_x) ||
		(direction.y == 0 && from.y == f_y) ||
		(direction.z == 0 && from.z == f_z)
	) {
		return {};
	}

	BlockPosition block_position{static_cast<int64_t>(f_x), static_cast<int64_t>(f_y), static_cast<int64_t>(f_z)};

	if (f_x != from.x && f_y != from.y && f_z != from.z) {
		const Block* block = world.block_at(block_position);
		if (block && block->is_visible()) {
			return std::tuple{block_position, BlockDirection::NONE, 0};
		}
	}

	int64_t step_x;
	float t_max_x;
	float t_delta_x = 1 / std::abs(direction.x);
	if (direction.x > 0) {
		step_x = 1;
		t_max_x = (std::ceil(from.x) - from.x) / direction.x;
	} else if (direction.x < 0) {
		step_x = -1;
		t_max_x = (f_x - from.x) / direction.x;
	} else {
		step_x = 0;
		t_max_x = std::numeric_limits<float>::infinity();
	}

	int64_t step_y;
	float t_max_y;
	float t_delta_y = 1 / std::abs(direction.y);
	if (direction.y > 0) {
		step_y = 1;
		t_max_y = (std::ceil(from.y) - from.y) / direction.y;
	} else if (direction.y < 0) {
		step_y = -1;
		t_max_y = (f_y - from.y) / direction.y;
	} else {
		step_y = 0;
		t_max_y = std::numeric_limits<float>::infinity();
	}

	int64_t step_z;
	float t_max_z;
	float t_delta_z = 1 / std::abs(direction.z);
	if (direction.z > 0) {
		step_z = 1;
		t_max_z = (std::ceil(from.z) - from.z) / direction.z;
	} else if (direction.z < 0) {
		step_z = -1;
		t_max_z = (f_z - from.z) / direction.z;
	} else {
		step_z = 0;
		t_max_z = std::numeric_limits<float>::infinity();
	}

	if (t_max_x == 0 && step_x == 1) {
		const Block* block = world.block_at(block_position);
		if (block && block->is_visible()) {
			return std::tuple{block_position, step_x == 1 ? BlockDirection::X_NEG : BlockDirection::X_POS, t_max_x};
		}
		t_max_x += t_delta_x;
	}

	if (t_max_y == 0 && step_y == 1) {
		const Block* block = world.block_at(block_position);
		if (block && block->is_visible()) {
			return std::tuple{block_position, step_y == 1 ? BlockDirection::Y_NEG : BlockDirection::Y_POS, t_max_y};
		}
		t_max_y += t_delta_y;
	}

	if (t_max_z == 0 && step_z == 1) {
		const Block* block = world.block_at(block_position);
		if (block && block->is_visible()) {
			return std::tuple{block_position, step_z == 1 ? BlockDirection::Z_NEG : BlockDirection::Z_POS, t_max_z};
		}
		t_max_z += t_delta_z;
	}

	while (true) {
		if (t_max_x <= t_max_y) {
			if (t_max_x <= t_max_z) {
				if (t_max_x >= distance)
					break;

				block_position.x += step_x;

				const Block* block = world.block_at(block_position);
				if (block && block->is_visible()) {
					return std::tuple{block_position, step_x == 1 ? BlockDirection::X_NEG : BlockDirection::X_POS, t_max_x};
				}

				t_max_x += t_delta_x;
			} else {
				if (t_max_z >= distance)
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
				if (t_max_z >= distance)
					break;

				block_position.z += step_z;

				const Block* block = world.block_at(block_position);
				if (block && block->is_visible()) {
					return std::tuple{block_position, step_z == 1 ? BlockDirection::Z_NEG : BlockDirection::Z_POS, t_max_z};
				}

				t_max_z += t_delta_z;
			} else {
				if (t_max_y >= distance)
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
