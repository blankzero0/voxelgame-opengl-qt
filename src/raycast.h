#ifndef VOXELGAME_OPENGL_QT_RAYCAST_H
#define VOXELGAME_OPENGL_QT_RAYCAST_H

#include <optional>
#include "geometry.h"
#include "world.h"

std::optional<std::pair<BlockPosition, BlockDirection>> raycast(const World& world, Point from, Vector direction, float distance);

#endif //VOXELGAME_OPENGL_QT_RAYCAST_H
