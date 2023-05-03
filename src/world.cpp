#include <cassert>
#include "world.h"
#include "arithmetic.h"

namespace {

std::pair<ChunkPosition, ChunkCoords> locate_block(const BlockPosition& block_position)
{
	int64_t dx = floor_div(block_position.x, Chunk::size);
	int64_t rx = floor_mod(block_position.x, Chunk::size);
	int64_t dy = floor_div(block_position.y, Chunk::size);
	int64_t ry = floor_mod(block_position.y, Chunk::size);
	int64_t dz = floor_div(block_position.z, Chunk::size);
	int64_t rz = floor_mod(block_position.z, Chunk::size);

	return {
			ChunkPosition{static_cast<int16_t>(dx), static_cast<int16_t>(dy), static_cast<int16_t>(dz)},
			ChunkCoords{static_cast<uint8_t>(rx), static_cast<uint8_t>(ry), static_cast<uint8_t>(rz)}
	};
}

}

World::World()
{
}

const Block* World::block_at(const BlockPosition& position) const
{
	auto [chunk_position, coords] = locate_block(position);

	auto *chunk = chunk_at(chunk_position);
	if (!chunk)
		return nullptr;

	return &chunk->get_block(coords);
}

const Chunk* World::chunk_at(const ChunkPosition& position) const
{
	std::lock_guard guard(chunks_mutex);
	auto it = chunks.find(position);
	if (it == chunks.end())
		return nullptr;

	return &it->second;
}

void World::add_chunk(const ChunkPosition& position, Chunk&& chunk)
{
	std::lock_guard guard(chunks_mutex);
	assert(!chunks.contains(position));
	auto it = chunks.insert({position, std::move(chunk)}).first;
	for (auto& listener : chunk_added_listeners) {
		listener(it->first, it->second);
	}
}

void World::add_chunk_added_listener(std::function<void(const ChunkPosition&, const Chunk&)>&& chunk_added_listener)
{
	World::chunk_added_listeners.push_back(std::move(chunk_added_listener));
}

void World::set_block(const BlockPosition& position, const Block& block)
{
	auto [chunk_position, coords] = locate_block(position);

	auto* chunk = const_cast<Chunk*>(chunk_at(chunk_position));
	chunk->set_block(coords, block);
}
