#ifndef VOXELGAME_OPENGL_QT_BLOCK_MESH_VERTEX_BUILDER_H
#define VOXELGAME_OPENGL_QT_BLOCK_MESH_VERTEX_BUILDER_H


#include <thread>
#include "geometry.h"
#include "block_vertex.h"
#include "world.h"
#include "blocking_queue.h"
#include "chunk_surrounder.h"
#include "vertex_producer.h"
#include "vertex_builder.h"

class BlockMeshVertexBuilder : public VertexBuilder<BlockVertex> {
public:
	explicit BlockMeshVertexBuilder(World& world);

	BlockMeshVertexBuilder(BlockMeshVertexBuilder&) = delete;
	BlockMeshVertexBuilder(BlockMeshVertexBuilder&&) = delete;
	BlockMeshVertexBuilder& operator=(BlockMeshVertexBuilder&) = delete;
	BlockMeshVertexBuilder& operator=(BlockMeshVertexBuilder&&) = delete;

	~BlockMeshVertexBuilder() override;

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


#endif //VOXELGAME_OPENGL_QT_BLOCK_MESH_VERTEX_BUILDER_H
