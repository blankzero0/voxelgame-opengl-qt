#ifndef VOXELGAME_OPENGL_QT_ARITHMETIC_H
#define VOXELGAME_OPENGL_QT_ARITHMETIC_H

#include <cstdint>

inline int64_t floor_div(int64_t a, int64_t b)
{
	return a < 0 ? (a - (b - 1)) / b : a / b;
}

inline int64_t floor_mod(int64_t a, int64_t b)
{
	return (a % b + b) % b;
}


#endif //VOXELGAME_OPENGL_QT_ARITHMETIC_H
