#include "geometry.h"
#include <cmath>

Vector operator-(const Vector& v)
{
	return {-v.x, -v.y, -v.z};
}

Vector operator*(float s, const Vector& v)
{
	return {s * v.x, s * v.y, s * v.z};
}

Vector operator+(const Vector& lhs, const Vector& rhs)
{
	return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

Point operator+(const Point& lhs, const Vector& rhs)
{
	return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

Vector rotation_to_forward_vector(float yaw, float pitch)
{
	return {
			std::sin(yaw) * std::cos(pitch),
			std::sin(pitch),
			-std::cos(yaw) * std::cos(pitch),
	};
}

Vector rotation_to_right_vector(float yaw, float pitch)
{
	return {
			std::cos(yaw),
			0,
			std::sin(yaw),
	};
}

Vector& operator+=(Vector& lhs, const Vector& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}

Point& operator+=(Point& lhs, const Vector& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}

Vector& operator-=(Vector& lhs, const Vector& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}

bool operator==(const ChunkPosition& lhs, const ChunkPosition& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

Vector2D angle_to_vector(float angle)
{
	return {std::cos(angle), std::sin(angle)};
}

float dot_product(const Vector2D& lhs, const Vector2D& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

std::size_t std::hash<ChunkPosition>::operator()(const ChunkPosition& chunk_position) const noexcept
{
	return std::hash<uint64_t>()(
			static_cast<uint64_t>(static_cast<uint16_t>(chunk_position.z)) << 32 |
					static_cast<uint64_t>(static_cast<uint16_t>(chunk_position.y)) << 16 |
					static_cast<uint64_t>(static_cast<uint16_t>(chunk_position.x))
	);
}
