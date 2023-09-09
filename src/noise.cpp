#include <cmath>
#include <numbers>
#include "noise.h"

namespace {

uint32_t wang_hash(uint32_t a)
{
	a = (a ^ 61) ^ (a >> 16);
	a = a + (a << 3);
	a = a ^ (a >> 4);
	a = a * 0x27d4eb2d;
	a = a ^ (a >> 15);
	return a;
}

// returns in range [-1, 1]
float random_value(const GridPoint2D& gp)
{
	uint32_t n = 19 * static_cast<uint32_t>(gp.x) + 47 * static_cast<uint32_t>(gp.y);
	uint32_t h = wang_hash(n);
	return static_cast<float>(h) / static_cast<float>(UINT32_MAX / 2) - 1;
}

Vector2D random_gradient(const GridPoint2D& gp)
{
	float a = random_value(gp) * std::numbers::pi_v<float>;
	return angle_to_vector(a);
}

float random_dot_product(const GridPoint2D& gp, const Vector2D& offset)
{
	Vector2D gradient = random_gradient(gp);
	return dot_product(gradient, offset);
}

float interpolate(float a, float b, float t)
{
	t = (3 - 2 * t) * t * t;
	return a + (b - a) * t;
}

}

float perlin_noise2d(const Point2D& p)
{
	GridPoint2D gp00 = {static_cast<int32_t>(std::floor(p.x)), static_cast<int32_t>(std::floor(p.y))};
	GridPoint2D gp10 = {gp00.x + 1, gp00.y + 0};
	GridPoint2D gp01 = {gp00.x + 0, gp00.y + 1};
	GridPoint2D gp11 = {gp00.x + 1, gp00.y + 1};

	Vector2D offset00{p.x - gp00.x, p.y - gp00.y};
	Vector2D offset10{p.x - gp10.x, p.y - gp10.y};
	Vector2D offset01{p.x - gp01.x, p.y - gp01.y};
	Vector2D offset11{p.x - gp11.x, p.y - gp11.y};

	float r00 = random_dot_product(gp00, offset00);
	float r10 = random_dot_product(gp10, offset10);
	float r01 = random_dot_product(gp01, offset01);
	float r11 = random_dot_product(gp11, offset11);

	float x0 = interpolate(r00, r10, offset00.x);
	float x1 = interpolate(r01, r11, offset00.x);

	return interpolate(x0, x1, offset00.y);
}

float fractal_perlin_noise2d(const Point2D& p)
{
	float noise = 0;
	float frequency = 1.0 / 128;
	float amplitude = 64.0;
	for (size_t octave = 0; octave < 3; ++octave) {
		noise += perlin_noise2d({p.x * frequency, p.y * frequency}) * amplitude;
		frequency *= 4;
		amplitude *= 0.25;
	}
	return noise;
}
