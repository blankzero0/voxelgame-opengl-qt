#ifndef VOXELGAME_OPENGL_QT_GRASS_BLOCK_H
#define VOXELGAME_OPENGL_QT_GRASS_BLOCK_H


#include "block.h"

class GrassBlock : public Block {
public:
	[[nodiscard]] bool is_visible() const override;
	[[nodiscard]] uint8_t texture_index_side() const override;
	[[nodiscard]] uint8_t texture_index_bottom() const override;
	[[nodiscard]] uint8_t texture_index() const override;
};


#endif //VOXELGAME_OPENGL_QT_GRASS_BLOCK_H
