#include <cmath>
#include "raycast.h"
#include "world.h"

std::optional<std::pair<BlockPosition, BlockDirection>> raycast(const World& world, Point from, Vector direction, float distance)
{
	constexpr float step_length = 0.01;
	
	float ray_length = 0;
	Vector ray_step = step_length * direction;

	Point to = from;
	while (ray_length <= distance) {
		BlockPosition position{static_cast<int64_t>(std::floor(to.x)), static_cast<int64_t>(std::floor(to.y)), static_cast<int64_t>(std::floor(to.z))};

		const Block* block = world.block_at(position);
		if (block && block->is_visible()) {
			BlockPosition from_position{static_cast<int64_t>(std::floor(from.x)), static_cast<int64_t>(std::floor(from.y)), static_cast<int64_t>(std::floor(from.z))};

			BlockDirection block_direction = BlockDirection::NONE;

			if (from_position.x > position.x) {
				block_direction = BlockDirection::X_POS;
			} else if (from_position.x < position.x) {
				block_direction = BlockDirection::X_NEG;
			} else if (from_position.y > position.y) {
				block_direction = BlockDirection::Y_POS;
			} else if (from_position.y < position.y) {
				block_direction = BlockDirection::Y_NEG;
			} else if (from_position.z > position.z) {
				block_direction = BlockDirection::Z_POS;
			} else if (from_position.z < position.z) {
				block_direction = BlockDirection::Z_NEG;
			}

			return std::pair{position, block_direction};
		}

		from = to;
		to = from + ray_step;
		ray_length += step_length;
	}

	return {};
}
