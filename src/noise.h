#ifndef VOXELGAME_OPENGL_QT_NOISE_H
#define VOXELGAME_OPENGL_QT_NOISE_H


#include "geometry.h"

[[nodiscard]] float perlin_noise2d(const Point2D& p);
[[nodiscard]] float fractal_perlin_noise2d(const Point2D& p);


#endif //VOXELGAME_OPENGL_QT_NOISE_H
