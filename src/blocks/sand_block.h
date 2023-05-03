#ifndef VOXELGAME_OPENGL_QT_SAND_BLOCK_H
#define VOXELGAME_OPENGL_QT_SAND_BLOCK_H


#include "block.h"

class SandBlock : public Block {
public:
	bool is_visible() const override;
	uint8_t texture_index() const override;
};


#endif //VOXELGAME_OPENGL_QT_SAND_BLOCK_H
