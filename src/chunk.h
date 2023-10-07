#ifndef VOXELGAME_OPENGL_QT_CHUNK_H
#define VOXELGAME_OPENGL_QT_CHUNK_H


#include <array>
#include <cstddef>
#include <functional>
#include "blocks/block.h"
#include "geometry.h"


enum BlockIndex : uint8_t {
	air = 0,
	grass,
	stone,
	water,
	sand,
	cobblestone,
};

extern const Block& player_placed_block;
extern const Block& player_unplaced_block;

class Chunk {
public:
	Chunk();
	explicit Chunk(const ChunkPosition& position);
	static constexpr size_t size = 32;

	[[nodiscard]] const Block& get_block(const ChunkCoords& coords) const;
	void set_block(const ChunkCoords& coords, const Block& block);
	void set_changed_listener(const std::function<void()>& changed_listener) const;
	void clear_changed_listener() const;
private:
	std::array<std::array<std::array<BlockIndex, size>, size>, size> blocks;
	mutable std::function<void()> changed_listener;
};


#endif //VOXELGAME_OPENGL_QT_CHUNK_H
