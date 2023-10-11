#include <cassert>
#include "block_mesh_vertex_builder.h"
#include <functional>

namespace {

void add_front_face(std::vector<BlockVertex>& vertices, uint8_t texture_index, float x, float y, float z)
{
	vertices.push_back({x + 0, y + 0, z + 1, 0, 0, texture_index, 0, 0, 1});
	vertices.push_back({x + 1, y + 0, z + 1, 1, 0, texture_index, 0, 0, 1});
	vertices.push_back({x + 1, y + 1, z + 1, 1, 1, texture_index, 0, 0, 1});
	vertices.push_back({x + 0, y + 1, z + 1, 0, 1, texture_index, 0, 0, 1});
}

void add_right_face(std::vector<BlockVertex>& vertices, const uint8_t texture_index, float x, float y, float z)
{
	vertices.push_back({x + 1, y + 0, z + 1, 0, 0, texture_index, 1, 0, 0});
	vertices.push_back({x + 1, y + 0, z + 0, 1, 0, texture_index, 1, 0, 0});
	vertices.push_back({x + 1, y + 1, z + 0, 1, 1, texture_index, 1, 0, 0});
	vertices.push_back({x + 1, y + 1, z + 1, 0, 1, texture_index, 1, 0, 0});
}

void add_back_face(std::vector<BlockVertex>& vertices, const uint8_t texture_index, float x, float y, float z)
{
	vertices.push_back({x + 1, y + 0, z + 0, 0, 0, texture_index, 0, 0, -1});
	vertices.push_back({x + 0, y + 0, z + 0, 1, 0, texture_index, 0, 0, -1});
	vertices.push_back({x + 0, y + 1, z + 0, 1, 1, texture_index, 0, 0, -1});
	vertices.push_back({x + 1, y + 1, z + 0, 0, 1, texture_index, 0, 0, -1});
}

void add_left_face(std::vector<BlockVertex>& vertices, const uint8_t texture_index, float x, float y, float z)
{
	vertices.push_back({x + 0, y + 0, z + 0, 0, 0, texture_index, -1, 0, 0});
	vertices.push_back({x + 0, y + 0, z + 1, 1, 0, texture_index, -1, 0, 0});
	vertices.push_back({x + 0, y + 1, z + 1, 1, 1, texture_index, -1, 0, 0});
	vertices.push_back({x + 0, y + 1, z + 0, 0, 1, texture_index, -1, 0, 0});
}

void add_top_face(std::vector<BlockVertex>& vertices, const uint8_t texture_index, float x, float y, float z)
{
	vertices.push_back({x + 0, y + 1, z + 1, 0, 0, texture_index, 0, 1, 0});
	vertices.push_back({x + 1, y + 1, z + 1, 1, 0, texture_index, 0, 1, 0});
	vertices.push_back({x + 1, y + 1, z + 0, 1, 1, texture_index, 0, 1, 0});
	vertices.push_back({x + 0, y + 1, z + 0, 0, 1, texture_index, 0, 1, 0});
}

void add_bottom_face(std::vector<BlockVertex>& vertices, const uint8_t texture_index, float x, float y, float z)
{
	vertices.push_back({x + 0, y + 0, z + 0, 0, 0, texture_index, 0, -1, 0});
	vertices.push_back({x + 1, y + 0, z + 0, 1, 0, texture_index, 0, -1, 0});
	vertices.push_back({x + 1, y + 0, z + 1, 1, 1, texture_index, 0, -1, 0});
	vertices.push_back({x + 0, y + 0, z + 1, 0, 1, texture_index, 0, -1, 0});
}

void add_block_faces(
		std::vector<BlockVertex>& vertices, uint8_t texture_index, uint8_t texture_index_side, uint8_t texture_index_bottom, const Chunk& chunk, const ChunkCoords coords, float x,
		float y, float z
)
{
	auto [cx, cy, cz] = coords;
	if (cz >= Chunk::size - 1 || !chunk.get_block({cx, cy, static_cast<uint8_t>(cz + 1)}).is_visible())
		add_front_face(vertices, texture_index_side, x, y, z);
	if (cx >= Chunk::size - 1 || !chunk.get_block({static_cast<uint8_t>(cx + 1), cy, cz}).is_visible())
		add_right_face(vertices, texture_index_side, x, y, z);
	if (cz <= 0 || !chunk.get_block({cx, cy, static_cast<uint8_t>(cz - 1)}).is_visible())
		add_back_face(vertices, texture_index_side, x, y, z);
	if (cx <= 0 || !chunk.get_block({static_cast<uint8_t>(cx - 1), cy, cz}).is_visible())
		add_left_face(vertices, texture_index_side, x, y, z);
	if (cy >= Chunk::size - 1 || !chunk.get_block({cx, static_cast<uint8_t>(cy + 1), cz}).is_visible())
		add_top_face(vertices, texture_index, x, y, z);
	if (cy <= 0 || !chunk.get_block({cx, static_cast<uint8_t>(cy - 1), cz}).is_visible())
		add_bottom_face(vertices, texture_index_bottom, x, y, z);
}

std::vector<BlockVertex> generate_vertices(const Chunk& chunk, const ChunkPosition& position)
{
	constexpr size_t vertices_per_cube = 6 * 6;

	std::vector<BlockVertex> vertices;
	vertices.reserve(Chunk::size * Chunk::size * Chunk::size * vertices_per_cube);

	for (uint8_t x = 0; x < Chunk::size; ++x) {
		for (uint8_t y = 0; y < Chunk::size; ++y) {
			for (uint8_t z = 0; z < Chunk::size; ++z) {
				ChunkCoords coords{x, y, z};
				const Block& block = chunk.get_block(coords);

				if (!block.is_visible())
					continue;

				constexpr int64_t chunk_size = Chunk::size;
				add_block_faces(
						vertices, block.texture_index(), block.texture_index_side(), block.texture_index_bottom(), chunk, coords,
						chunk_size * position.x + x, chunk_size * position.y + y, chunk_size * position.z + z
				);
			}
		}
	}

	vertices.shrink_to_fit();
	return vertices;
}

}

BlockMeshVertexBuilder::BlockMeshVertexBuilder(World& world)
		: world(world)
{
	world.add_chunk_added_listener([this](const ChunkPosition& position, const Chunk& chunk) { this->on_chunk_added(position, chunk); });

	unsigned int thread_count = std::jthread::hardware_concurrency();
	threads.reserve(thread_count);
	for (size_t i = 0; i < thread_count; ++i)
		threads.emplace_back(std::bind_front(&BlockMeshVertexBuilder::run, this));
}

void BlockMeshVertexBuilder::request_chunk(const ChunkPosition& position)
{
	const Chunk* chunk = world.chunk_at(position);
	if (chunk) {
		supply_chunk(position, *chunk);
	} else {
		std::lock_guard guard(waiting_requests_mutex);
		waiting_requests.insert(position);
	}
}

void BlockMeshVertexBuilder::run(const std::stop_token& stop_token)
{
	while (!stop_token.stop_requested()) {
		auto opt = queue.take();
		if (!opt)
			continue;
		auto [position, chunk] = *opt;

		get_vertex_consumer().consume_vertices({position, generate_vertices(chunk, position)});
	}
}

BlockMeshVertexBuilder::~BlockMeshVertexBuilder()
{
	for (auto& thread : threads) {
		thread.request_stop();
	}
}

void BlockMeshVertexBuilder::supply_chunk(const ChunkPosition& position, const Chunk& chunk)
{
	queue.put(std::pair<ChunkPosition, std::reference_wrapper<const Chunk>>(position, chunk));
}

void BlockMeshVertexBuilder::on_chunk_added(const ChunkPosition& position, const Chunk& chunk)
{
	std::lock_guard guard(waiting_requests_mutex);

	auto it = waiting_requests.find(position);
	if (it == waiting_requests.end()) {
		assert(!waiting_requests.contains(position));
		return;
	}
	waiting_requests.erase(it);

	supply_chunk(position, chunk);
}
