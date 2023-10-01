#ifndef VOXELGAME_OPENGL_QT_WORLD_H
#define VOXELGAME_OPENGL_QT_WORLD_H


#include <unordered_map>
#include <mutex>
#include "geometry.h"
#include "chunk.h"

class World {
public:
	World();
	const Block* block_at(const BlockPosition& position) const;
	const Chunk* chunk_at(const ChunkPosition& position) const;
	void add_chunk(const ChunkPosition& position, Chunk&& chunk);
	void add_chunk_added_listener(std::function<void(const ChunkPosition&, const Chunk&)>&& chunk_added_listener);

	void set_block(const BlockPosition& position, const Block& block);
private:
	std::unordered_map<ChunkPosition, Chunk> chunks;
	mutable std::mutex chunks_mutex;
	std::vector<std::function<void(const ChunkPosition&, const Chunk&)>> chunk_added_listeners;
	mutable std::mutex listeners_mutex;
};


#endif //VOXELGAME_OPENGL_QT_WORLD_H
