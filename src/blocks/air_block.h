#ifndef VOXELGAME_OPENGL_QT_AIR_BLOCK_H
#define VOXELGAME_OPENGL_QT_AIR_BLOCK_H


#include "block.h"

class AirBlock : public Block {
public:
	[[nodiscard]] bool is_visible() const override;
	[[nodiscard]] uint8_t texture_index() const override;
};


#endif //VOXELGAME_OPENGL_QT_AIR_BLOCK_H
