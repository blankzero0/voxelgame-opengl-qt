#ifndef VOXELGAME_OPENGL_QT_BLOCK_H
#define VOXELGAME_OPENGL_QT_BLOCK_H


#include <cstdint>

class Block {
public:
	Block() = default;
	Block(Block&) = delete;
	Block& operator=(Block&) = delete;
	Block(Block&&) = delete;
	Block& operator=(Block&&) = delete;
	virtual ~Block() = default;

	[[nodiscard]] virtual bool is_visible() const = 0;
	[[nodiscard]] virtual uint8_t texture_index() const = 0;
	[[nodiscard]] virtual uint8_t texture_index_side() const;
	[[nodiscard]] virtual uint8_t texture_index_bottom() const;
};


#endif //VOXELGAME_OPENGL_QT_BLOCK_H
