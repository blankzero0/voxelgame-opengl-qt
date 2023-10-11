#include <cassert>
#include "water_mesh_vertex_builder.h"
#include <functional>

namespace {

void add_top_face(std::vector<WaterVertex>& vertices, float x, float y, float z)
{
	vertices.push_back({x + 0, y, z + 1, 0, 0});
	vertices.push_back({x + 1, y, z + 1, 1, 0});
	vertices.push_back({x + 1, y, z + 0, 1, 1});
	vertices.push_back({x + 0, y, z + 0, 0, 1});
}

std::vector<WaterVertex> generate_vertices(const Chunk& chunk, const ChunkPosition& position)
{
	const std::vector<WaterColumn>& water = chunk.get_water();

	int64_t cx = position.x * Chunk::size;
	int64_t cy = position.y * Chunk::size;
	int64_t cz = position.z * Chunk::size;

	std::vector<WaterVertex> vertices;
	vertices.reserve(water.size() * 4);
	for (const WaterColumn& water_column : water) {
		add_top_face(
				vertices,
				cx + water_column.coords.x,
				cy + water_column.coords.y + water_column.height,
				cz + water_column.coords.z
		);
	}

	vertices.shrink_to_fit();
	return vertices;
}

}

WaterMeshVertexBuilder::WaterMeshVertexBuilder(World& world)
		: world(world)
{
	world.add_chunk_added_listener([this](const ChunkPosition& position, const Chunk& chunk) { this->on_chunk_added(position, chunk); });

	unsigned int thread_count = std::jthread::hardware_concurrency();
	threads.reserve(thread_count);
	for (size_t i = 0; i < thread_count; ++i)
		threads.emplace_back(std::bind_front(&WaterMeshVertexBuilder::run, this));
}

void WaterMeshVertexBuilder::request_chunk(const ChunkPosition& position)
{
	const Chunk* chunk = world.chunk_at(position);
	if (chunk) {
		supply_chunk(position, *chunk);
	} else {
		std::lock_guard guard(waiting_requests_mutex);
		waiting_requests.insert(position);
	}
}

void WaterMeshVertexBuilder::run(const std::stop_token& stop_token)
{
	while (!stop_token.stop_requested()) {
		auto opt = queue.take();
		if (!opt)
			continue;
		auto [position, chunk] = *opt;

		get_vertex_consumer().consume_vertices({position, generate_vertices(chunk, position)});
	}
}

WaterMeshVertexBuilder::~WaterMeshVertexBuilder()
{
	for (auto& thread : threads) {
		thread.request_stop();
	}
}

void WaterMeshVertexBuilder::supply_chunk(const ChunkPosition& position, const Chunk& chunk)
{
	queue.put(std::pair<ChunkPosition, std::reference_wrapper<const Chunk>>(position, chunk));
}

void WaterMeshVertexBuilder::on_chunk_added(const ChunkPosition& position, const Chunk& chunk)
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

WaterMeshVertexBuilder::WaterUpdateConnector::WaterUpdateConnector(World& world, WaterMeshVertexBuilder& vertex_builder)
	: world(world), vertex_builder(vertex_builder)
{
}

void WaterMeshVertexBuilder::WaterUpdateConnector::connect(const ChunkPosition& position)
{

}

void WaterMeshVertexBuilder::WaterUpdateConnector::disconnect(const ChunkPosition& position)
{

}
