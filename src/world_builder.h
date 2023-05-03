#ifndef VOXELGAME_OPENGL_QT_WORLD_BUILDER_H
#define VOXELGAME_OPENGL_QT_WORLD_BUILDER_H


#include <optional>
#include <thread>
#include "geometry.h"
#include "chunk_surrounder.h"
#include "world.h"
#include "blocking_queue.h"

class WorldBuilder : public ChunkRequester<WorldBuilder>, public CenterChangeListener<WorldBuilder> {
public:
	void set_center(const Point& center);
	~WorldBuilder() override;
	explicit WorldBuilder(World& world);

	void run(const std::stop_token& stop_token);

	void request_chunk(const ChunkPosition& position);
	void on_center_changed(const ChunkPosition& center);

private:
	World& world;
	ChunkSurrounder<8, WorldBuilder, WorldBuilder> chunk_surrounder;
	BlockingQueue<ChunkPosition> requests;
	std::vector<std::jthread> threads;
};


#endif //VOXELGAME_OPENGL_QT_WORLD_BUILDER_H
