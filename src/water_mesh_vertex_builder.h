#ifndef VOXELGAME_OPENGL_QT_WATER_MESH_VERTEX_BUILDER_H
#define VOXELGAME_OPENGL_QT_WATER_MESH_VERTEX_BUILDER_H


#include <thread>
#include "geometry.h"
#include "world.h"
#include "blocking_queue.h"
#include "chunk_surrounder.h"
#include "water_vertex.h"
#include "vertex_builder.h"
#include "update_connector.h"

class WaterMeshVertexBuilder : public VertexBuilder<WaterVertex> {
public:
	class WaterUpdateConnector : public UpdateConnector {
	public:
		WaterUpdateConnector(World& world, WaterMeshVertexBuilder& vertex_builder);

		void connect(const ChunkPosition& position) override;
		void disconnect(const ChunkPosition& position) override;

	private:
		World& world;
		WaterMeshVertexBuilder& vertex_builder;
	};

	explicit WaterMeshVertexBuilder(World& world);

	WaterMeshVertexBuilder(WaterMeshVertexBuilder&) = delete;
	WaterMeshVertexBuilder(WaterMeshVertexBuilder&&) = delete;
	WaterMeshVertexBuilder& operator=(WaterMeshVertexBuilder&) = delete;
	WaterMeshVertexBuilder& operator=(WaterMeshVertexBuilder&&) = delete;

	~WaterMeshVertexBuilder() override;

	void request_chunk(const ChunkPosition& position) override;
	void supply_chunk(const ChunkPosition& position, const Chunk& chunk);
	void on_chunk_added(const ChunkPosition& position, const Chunk& chunk);

private:
	World& world;
	BlockingQueue<std::pair<ChunkPosition, std::reference_wrapper<const Chunk>>> queue;
	std::vector<std::jthread> threads;
	std::unordered_set<ChunkPosition> waiting_requests;
	std::mutex waiting_requests_mutex;
	void run(const std::stop_token& stop_token);
};


#endif //VOXELGAME_OPENGL_QT_WATER_MESH_VERTEX_BUILDER_H
