#include "chunk_surrounder.h"



ChunkSurrounder::ChunkSurrounder(
		unsigned int view_distance,
		ChunkRequester& chunk_requester, CenterChangeListener& center_change_listener
)
		: view_distance(view_distance), chunk_requester(chunk_requester), center_change_listener(center_change_listener)
{}

void ChunkSurrounder::set_center(const Point& center_point)
{
	ChunkPosition center{
			static_cast<int16_t>(std::floor(center_point.x / Chunk::size)),
			static_cast<int16_t>(std::floor(center_point.y / Chunk::size)),
			static_cast<int16_t>(std::floor(center_point.z / Chunk::size)),
	};

	if (this->last_center == center)
		return;

	auto last_center_opt = this->last_center;

	this->last_center = center;

	center_change_listener.on_center_changed(center);

	int distance_signed = view_distance;

	if (!last_center_opt) {
		for (int x = center.x - distance_signed; x <= center.x + distance_signed; ++x) {
			for (int y = center.y - distance_signed; y <= center.y + distance_signed; ++y) {
				for (int z = center.z - distance_signed; z <= center.z + distance_signed; ++z) {
					chunk_requester.request_chunk({static_cast<int16_t>(x), static_cast<int16_t>(y), static_cast<int16_t>(z)});
				}
			}
		}

		return;
	}

	auto last_center = *last_center_opt;

	std::unordered_set<ChunkPosition> positions_to_request;

	auto [x_from, x_to] = center.x >= last_center.x ?
			std::tuple{last_center.x + distance_signed + 1, center.x + distance_signed} :
			std::tuple{center.x - distance_signed, last_center.x - distance_signed - 1};
	for (int x = x_from; x <= x_to; ++x) {
		for (int y = center.y - distance_signed; y <= center.y + distance_signed; ++y) {
			for (int z = center.z - distance_signed; z <= center.z + distance_signed; ++z) {
				positions_to_request.insert({static_cast<int16_t>(x), static_cast<int16_t>(y), static_cast<int16_t>(z)});
			}
		}
	}

	auto [y_from, y_to] = center.y >= last_center.y ?
			std::tuple{last_center.y + distance_signed + 1, center.y + distance_signed} :
			std::tuple{center.y - distance_signed, last_center.y - distance_signed - 1};
	for (int y = y_from; y <= y_to; ++y) {
		for (int x = center.x - distance_signed; x <= center.x + distance_signed; ++x) {
			for (int z = center.z - distance_signed; z <= center.z + distance_signed; ++z) {
				positions_to_request.insert({static_cast<int16_t>(x), static_cast<int16_t>(y), static_cast<int16_t>(z)});
			}
		}
	}

	auto [z_from, z_to] = center.z >= last_center.z ?
			std::tuple{last_center.z + distance_signed + 1, center.z + distance_signed} :
			std::tuple{center.z - distance_signed, last_center.z - distance_signed - 1};
	for (int z = z_from; z <= z_to; ++z) {
		for (int x = center.x - distance_signed; x <= center.x + distance_signed; ++x) {
			for (int y = center.y - distance_signed; y <= center.y + distance_signed; ++y) {
				positions_to_request.insert({static_cast<int16_t>(x), static_cast<int16_t>(y), static_cast<int16_t>(z)});
			}
		}
	}

	for (const ChunkPosition& pos: positions_to_request) {
		chunk_requester.request_chunk(pos);
	}
}
