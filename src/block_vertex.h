#ifndef VOXELGAME_OPENGL_QT_BLOCK_VERTEX_H
#define VOXELGAME_OPENGL_QT_BLOCK_VERTEX_H

#include <cstdint>
#include "geometry.h"

struct BlockVertex {
	Point position;
	Point2D texture_coords;
	uint8_t texture_index;
	Vector normal;
};

#endif //VOXELGAME_OPENGL_QT_BLOCK_VERTEX_H
