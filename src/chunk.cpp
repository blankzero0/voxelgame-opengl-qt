#include <random>
#include <noise/noise.h>
#include <stdexcept>
#include "chunk.h"
#include "blocks/air_block.h"
#include "blocks/grass_block.h"
#include "blocks/stone_block.h"
#include "blocks/water_block.h"
#include "blocks/sand_block.h"
#include "blocks/cobblestone_block.h"


namespace {

const AirBlock air_block;
const GrassBlock grass_block;
const StoneBlock stone_block;
const WaterBlock water_block;
const SandBlock sand_block;
const CobblestoneBlock cobblestone_block;

}

const Block& player_unplaced_block = air_block;
const Block& player_placed_block = cobblestone_block;

namespace {

const std::array all_blocks = std::to_array<std::reference_wrapper<const Block>>({
	air_block,
	grass_block,
	stone_block,
	water_block,
	sand_block,
	cobblestone_block,
});

const noise::module::Perlin perlin;

BlockIndex select_block(int64_t x, int64_t z, int64_t y, int64_t terrain_height)
{
	if (y > terrain_height) {
		if (y < 0) {
			return water;
		} else {
			return air;
		}
	}

	if (y == terrain_height) {
		if (y >= 2) {
			return grass;
		} else {
			return sand;
		}
	}

	return stone;
}

template<size_t... Ys>
std::array<BlockIndex, Chunk::size> make_line(const ChunkPosition& position, int64_t x, int64_t z, std::index_sequence<Ys...>)
{
	int64_t terrain_height = std::round(
			perlin.GetValue(static_cast<double>(x) / 16, 0, static_cast<double>(z) / 16) * 4 +
					perlin.GetValue(static_cast<double>(x) / 64, 0, static_cast<double>(z) / 64) * 16
	);

	return {select_block(x, z, position.y * Chunk::size + Ys, terrain_height)...};
}

template<size_t... Zs>
std::array<std::array<BlockIndex, Chunk::size>, Chunk::size> make_plane(const ChunkPosition& position, int64_t x, std::index_sequence<Zs...>)
{
	return {make_line(position, x, position.z * Chunk::size + Zs, std::make_index_sequence<Chunk::size>{})...};
}

template<size_t... Xs>
std::array<std::array<std::array<BlockIndex, Chunk::size>, Chunk::size>, Chunk::size>
make_blocks(const ChunkPosition& position, std::index_sequence<Xs...>)
{
	return {make_plane(position, position.x * Chunk::size + Xs, std::make_index_sequence<Chunk::size>{})...};
}

template<size_t... Is>
std::array<std::array<std::array<BlockIndex, Chunk::size>, Chunk::size>, Chunk::size>
make_empty(std::index_sequence<Is...>)
{
	return {((void)Is, air)...};
}

std::array<std::array<std::array<BlockIndex, Chunk::size>, Chunk::size>, Chunk::size>
make_empty()
{
	return make_empty(std::make_index_sequence<Chunk::size * Chunk::size * Chunk::size>{});
}

template<size_t... Is>
std::array<std::array<std::array<BlockIndex, Chunk::size>, Chunk::size>, Chunk::size>
make_full(std::index_sequence<Is...>)
{
	return {((void)Is, stone)...};
}

std::array<std::array<std::array<BlockIndex, Chunk::size>, Chunk::size>, Chunk::size>
make_full()
{
	return make_full(std::make_index_sequence<Chunk::size * Chunk::size * Chunk::size>{});
}

std::array<std::array<std::array<BlockIndex, Chunk::size>, Chunk::size>, Chunk::size> make_blocks(const ChunkPosition& position)
{
	if (position.y > 3)
		return make_empty();
	if (position.y < -3)
		return make_full();

	return make_blocks(position, std::make_index_sequence<Chunk::size>{});
}

}

Chunk::Chunk(const ChunkPosition& position)
		: blocks(make_blocks(position))
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

	if (changed_listener) {
		changed_listener();
	}
}

void Chunk::set_changed_listener(const std::function<void()>& changed_listener) const
{
	assert(Chunk::changed_listener == nullptr);
	Chunk::changed_listener = changed_listener;
}

void Chunk::clear_changed_listener() const
{
	changed_listener = nullptr;
}
