#include "world_builder.h"

void WorldBuilder::set_center(const Point& center)
{
	chunk_surrounder.set_center(center);
}

WorldBuilder::WorldBuilder(World& world)
	: chunk_surrounder(*this, *this), world(world)
{
	unsigned int thread_count = std::jthread::hardware_concurrency();
	threads.reserve(thread_count);
	for (size_t i = 0; i < thread_count; ++i)
		threads.emplace_back(&WorldBuilder::run, this);
}

void WorldBuilder::request_chunk(const ChunkPosition& position)
{
	if (!world.chunk_at(position)) {
		requests.put(position);
	}
}

void WorldBuilder::on_center_changed(const ChunkPosition&)
{
}

void WorldBuilder::run(const std::stop_token& stop_token)
{
	while (!stop_token.stop_requested()) {
		auto opt = requests.take();
		if (!opt)
			continue;
		ChunkPosition position = *opt;

		world.add_chunk(position, Chunk(position));
	}
}

WorldBuilder::~WorldBuilder()
{
	for (auto& thread : threads) {
		thread.request_stop();
	}
}
