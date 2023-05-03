#ifndef VOXELGAME_OPENGL_QT_COBBLESTONE_BLOCK_H
#define VOXELGAME_OPENGL_QT_COBBLESTONE_BLOCK_H


#include "block.h"

class CobblestoneBlock : public Block {
public:
	[[nodiscard]] bool is_visible() const override;
	[[nodiscard]] uint8_t texture_index() const override;
};


#endif //VOXELGAME_OPENGL_QT_COBBLESTONE_BLOCK_H
