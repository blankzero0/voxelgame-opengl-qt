#ifndef VOXELGAME_OPENGL_QT_CHUNK_MESH_VERTEX_BUILDER_H
#define VOXELGAME_OPENGL_QT_CHUNK_MESH_VERTEX_BUILDER_H


#include <thread>
#include "geometry.h"
#include "vertex.h"
#include "world.h"
#include "blocking_queue.h"
#include "chunk_surrounder.h"

class ChunkMeshVertexBuilder : public ChunkRequester<ChunkMeshVertexBuilder> {
public:
	using VertexConsumer = std::function<void(std::pair<ChunkPosition, std::vector<Vertex>>&&)>;

	explicit ChunkMeshVertexBuilder(World& world);

	ChunkMeshVertexBuilder(ChunkMeshVertexBuilder&) = delete;
	ChunkMeshVertexBuilder(ChunkMeshVertexBuilder&&) = delete;
	ChunkMeshVertexBuilder& operator=(ChunkMeshVertexBuilder&) = delete;
	ChunkMeshVertexBuilder& operator=(ChunkMeshVertexBuilder&&) = delete;

	~ChunkMeshVertexBuilder() override;

	void request_chunk(const ChunkPosition& position);

	void supply_chunk(const ChunkPosition& position, const Chunk& chunk);
	void set_vertex_consumer(VertexConsumer&& vertex_consumer);

	void on_chunk_added(const ChunkPosition& position, const Chunk& chunk);

private:
	World& world;
	VertexConsumer vertex_consumer;
	BlockingQueue<std::pair<ChunkPosition, std::reference_wrapper<const Chunk>>> queue;
	std::vector<std::jthread> threads;
	std::unordered_set<ChunkPosition> waiting_requests;
	std::mutex waiting_requests_mutex;
	void run(const std::stop_token& stop_token);
};


#endif //VOXELGAME_OPENGL_QT_CHUNK_MESH_VERTEX_BUILDER_H
