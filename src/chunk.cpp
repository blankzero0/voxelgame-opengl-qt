#include <stdexcept>
#include <cmath>
#include <cassert>
#include "chunk.h"
#include "blocks/air_block.h"
#include "blocks/grass_block.h"
#include "blocks/stone_block.h"
#include "blocks/sand_block.h"
#include "blocks/cobblestone_block.h"
#include "noise.h"


namespace {

const AirBlock air_block;
const GrassBlock grass_block;
const StoneBlock stone_block;
const SandBlock sand_block;
const CobblestoneBlock cobblestone_block;

}

const Block& player_unplaced_block = air_block;
const Block& player_placed_block = cobblestone_block;

namespace {

template <typename T, size_t N>
using Array3D = std::array<std::array<std::array<T, N>, N>, N>;
using Blocks = Array3D<BlockIndex, Chunk::size>;

const std::array all_blocks = std::to_array<std::reference_wrapper<const Block>>({
	air_block,
	grass_block,
	stone_block,
	sand_block,
	cobblestone_block,
});

BlockIndex select_block(int64_t x, int64_t z, int64_t y, int64_t terrain_height)
{
	if (y >= terrain_height) {
		return air;
	}

	if (y == terrain_height - 1) {
		if (y >= 2) {
			return grass;
		} else {
			return sand;
		}
	}

	return stone;
}

Blocks make_empty()
{
	Blocks blocks;
	std::fill_n(blocks[0][0].data(), Chunk::size * Chunk::size * Chunk::size, air);
	return blocks;
}

Blocks make_full()
{
	Blocks blocks;
	std::fill_n(blocks[0][0].data(), Chunk::size * Chunk::size * Chunk::size, stone);
	return blocks;
}

std::pair<Blocks, std::vector<WaterColumn>> make_terrain(const ChunkPosition& position)
{
	if (position.y > 3)
		return {make_empty(), {}};
	if (position.y < -3)
		return {make_full(), {}};

	Blocks blocks;
	std::vector<WaterColumn> water;

	for (size_t cx = 0; cx < Chunk::size; ++cx) {
		int64_t x = static_cast<int64_t>(position.x * Chunk::size) + static_cast<int64_t>(cx);
		for (size_t cz = 0; cz < Chunk::size; ++cz) {
			int64_t z = static_cast<int64_t>(position.z * Chunk::size) + static_cast<int64_t>(cz);

			int64_t terrain_height = std::round(
					fractal_perlin_noise2d({static_cast<float>(x), static_cast<float>(z)})
			);

			for (size_t cy = 0; cy < Chunk::size; ++cy) {
				int64_t y = static_cast<int64_t>(position.y * Chunk::size) + static_cast<int64_t>(cy);

				blocks[cx][cz][cy] = select_block(x, z, y, terrain_height);
			}

			if (terrain_height < 0) {
				int64_t chunk_bottom = static_cast<int64_t>(position.y * Chunk::size);
				int64_t chunk_top = chunk_bottom + static_cast<int64_t>(Chunk::size);
				uint8_t base = terrain_height - chunk_bottom + 1;
				float height = std::min(static_cast<float>(chunk_top), -0.125f) - base;
				water.emplace_back(ChunkCoords{static_cast<uint8_t>(cx), base, static_cast<uint8_t>(cz)}, height);
			}
		}
	}

	return {blocks, water};
}

}

Chunk::Chunk()
		: blocks(make_empty()), water()
{
}

Chunk::Chunk(const ChunkPosition& position)
		: Chunk(make_terrain(position))
{
}

const Block& Chunk::get_block(const ChunkCoords& coords) const
{
	return all_blocks.at(blocks[coords.x][coords.z][coords.y]);
}

void Chunk::set_block(const ChunkCoords& coords, const Block& block)
{
	auto it = std::find_if(all_blocks.begin(), all_blocks.end(), [&block](const Block& b){ return &b == &block; });
	if (it == all_blocks.end())
		throw std::invalid_argument("invalid block");

	auto block_index = static_cast<BlockIndex>(it - all_blocks.begin());
	blocks.at(coords.x).at(coords.z).at(coords.y) = block_index;

	if (blocks_changed_listener) {
		blocks_changed_listener();
	}
}

void Chunk::set_blocks_changed_listener(const std::function<void()>& changed_listener) const
{
	assert(Chunk::blocks_changed_listener == nullptr);
	Chunk::blocks_changed_listener = changed_listener;
}

void Chunk::clear_blocks_changed_listener() const
{
	blocks_changed_listener = nullptr;
}

Chunk::Chunk(const std::pair<std::array<std::array<std::array<BlockIndex, size>, size>, size>, std::vector<WaterColumn>>& members)
	: blocks(members.first), water(members.second)
{

}

const std::vector<WaterColumn>& Chunk::get_water() const
{
	return water;
}
