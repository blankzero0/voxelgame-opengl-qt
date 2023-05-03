#ifndef VOXELGAME_OPENGL_QT_STONE_BLOCK_H
#define VOXELGAME_OPENGL_QT_STONE_BLOCK_H


#include "block.h"

class StoneBlock : public Block {
public:
	bool is_visible() const override;
	uint8_t texture_index() const override;
};


#endif //VOXELGAME_OPENGL_QT_STONE_BLOCK_H
