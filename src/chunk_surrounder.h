#ifndef VOXELGAME_OPENGL_QT_CHUNK_SURROUNDER_H
#define VOXELGAME_OPENGL_QT_CHUNK_SURROUNDER_H


#include <optional>
#include <cmath>
#include <unordered_set>
#include "geometry.h"
#include "chunk.h"


template<typename T>
class CenterChangeListener {
public:
	CenterChangeListener() = default;
	CenterChangeListener(CenterChangeListener&) = delete;
	CenterChangeListener(CenterChangeListener&&) = delete;
	CenterChangeListener& operator=(CenterChangeListener&) = delete;
	CenterChangeListener& operator=(CenterChangeListener&&) = delete;
	virtual ~CenterChangeListener() = default;

	void on_center_changed(const ChunkPosition& center)
	{
		static_cast<T&>(*this).on_center_changed(center);
	}
};

template<typename T>
class ChunkRequester {
public:
	ChunkRequester() = default;
	ChunkRequester(ChunkRequester&) = delete;
	ChunkRequester(ChunkRequester&&) = delete;
	ChunkRequester& operator=(ChunkRequester&) = delete;
	ChunkRequester& operator=(ChunkRequester&&) = delete;
	virtual ~ChunkRequester() = default;

	void request_chunk(const ChunkPosition& center)
	{
		static_cast<T&>(*this).request_chunk(center);
	}
};

template<unsigned int, typename ConcreteChunkRequester, typename ConcreteCenterChangeListener>
class ChunkSurrounder {
public:
	void set_center(const Point& center);
	ChunkSurrounder(
			ChunkRequester<ConcreteChunkRequester>& chunk_requester, CenterChangeListener<ConcreteCenterChangeListener>& center_change_listener
	);

private:
	CenterChangeListener<ConcreteCenterChangeListener>& center_change_listener;
	ChunkRequester<ConcreteChunkRequester>& chunk_requester;
	std::optional<ChunkPosition> last_center;
};

template<unsigned int view_distance, typename ConcreteChunkRequester, typename ConcreteCenterChangeListener>
ChunkSurrounder<view_distance, ConcreteChunkRequester, ConcreteCenterChangeListener>::ChunkSurrounder(
		ChunkRequester<ConcreteChunkRequester>& chunk_requester, CenterChangeListener<ConcreteCenterChangeListener>& center_change_listener
)
		:chunk_requester(chunk_requester), center_change_listener(center_change_listener)
{}

template<unsigned int view_distance, typename ConcreteChunkRequester, typename ConcreteCenterChangeListener>
void ChunkSurrounder<view_distance, ConcreteChunkRequester, ConcreteCenterChangeListener>::set_center(const Point& center_point)
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

	constexpr int distance_signed = view_distance;

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


#endif //VOXELGAME_OPENGL_QT_CHUNK_SURROUNDER_H
