#ifndef VOXELGAME_OPENGL_QT_GEOMETRY_H
#define VOXELGAME_OPENGL_QT_GEOMETRY_H

#include <cstdint>
#include <functional>

template<typename T>
struct Base3D {
	T x, y, z;
};

struct Point : public Base3D<float> {
};

struct Point2D {
	float x, y;
};

struct Vector : public Base3D<float> {
};

Vector operator-(const Vector& v);
Vector operator*(float s, const Vector& v);
Vector operator+(const Vector& lhs, const Vector& rhs);
Point operator+(const Point& lhs, const Vector& rhs);
Vector& operator+=(Vector& lhs, const Vector& rhs);
Vector& operator-=(Vector& lhs, const Vector& rhs);
Point& operator+=(Point& lhs, const Vector& rhs);

Vector rotation_to_forward_vector(float yaw, float pitch);
Vector rotation_to_right_vector(float yaw, float pitch);

struct ChunkCoords {
	uint8_t x, y, z;
};

struct ChunkPosition {
	int16_t x, y, z;

	template<size_t I>
	[[nodiscard]] int16_t get() const;
};

template<>
inline int16_t ChunkPosition::get<0>() const
{ return x; }

template<>
inline int16_t ChunkPosition::get<1>() const
{ return y; }

template<>
inline int16_t ChunkPosition::get<2>() const
{ return z; }

bool operator==(const ChunkPosition& lhs, const ChunkPosition& rhs);

template<>
struct std::hash<ChunkPosition> {
	std::size_t operator()(const ChunkPosition& chunk_position) const noexcept;
};


struct BlockPosition {
	int64_t x, y, z;
};

enum BlockDirection {
	X_POS, X_NEG,
	Z_POS, Z_NEG,
	Y_POS, Y_NEG,
	NONE,
};

#endif //VOXELGAME_OPENGL_QT_GEOMETRY_H
