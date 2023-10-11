#ifndef VOXELGAME_OPENGL_QT_WORLD_BUILDER_H
#define VOXELGAME_OPENGL_QT_WORLD_BUILDER_H


#include <optional>
#include <thread>
#include "geometry.h"
#include "chunk_surrounder.h"
#include "world.h"
#include "blocking_queue.h"

class WorldBuilder : public ChunkRequester, public CenterChangeListener {
public:
	WorldBuilder(WorldBuilder&) = delete;
	WorldBuilder(WorldBuilder&&) = delete;
	WorldBuilder& operator=(WorldBuilder&) = delete;
	WorldBuilder& operator=(WorldBuilder&&) = delete;

	void set_center(const Point& center);
	~WorldBuilder() override;
	explicit WorldBuilder(World& world);

	void run(const std::stop_token& stop_token);

	void request_chunk(const ChunkPosition& position) override;
	void on_center_changed(const ChunkPosition& center) override;

private:
	static constexpr size_t generation_distance = 8;

	World& world;
	ChunkSurrounder chunk_surrounder;
	BlockingQueue<ChunkPosition> requests;
	std::vector<std::jthread> threads;
};


#endif //VOXELGAME_OPENGL_QT_WORLD_BUILDER_H
