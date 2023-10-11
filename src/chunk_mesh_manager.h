#ifndef VOXELGAME_OPENGL_QT_CHUNK_MESH_MANAGER_H
#define VOXELGAME_OPENGL_QT_CHUNK_MESH_MANAGER_H


#include "geometry.h"
#include "synchronized_queue.h"
#include "multimap3d.h"
#include "chunk.h"
#include "chunk_surrounder.h"
#include "time_bounded_gl_executor.h"
#include "vertex_builder.h"
#include "world.h"

template <typename Mesh, typename Vertex>
class ChunkMeshManager : CenterChangeListener, VertexConsumer<Vertex> {
public:
	ChunkMeshManager(size_t view_distance, TimeBoundedGLExecutor& gl_executor, VertexBuilder<Vertex>& vertex_builder, VertexBuilder<Vertex>& update_vertex_builder, const World& world);

	void set_center(const Point& center_point);

	std::vector<Mesh>& get_meshes();

private:
	struct MeshWrapper {
		void free();

		ChunkPosition position;
		Mesh& mesh;
		const Chunk& chunk;
		std::vector<std::reference_wrapper<Mesh>>& free_list;
		std::unordered_map<ChunkPosition, std::reference_wrapper<Mesh>>& mesh_position_map;
	};

	struct UpdateVertexConsumer : VertexConsumer<Vertex>  {
		UpdateVertexConsumer(TimeBoundedGLExecutor& executor, std::unordered_map<ChunkPosition, std::reference_wrapper<Mesh>>& map)
			: gl_executor(executor), mesh_position_map(map)
		{}
		TimeBoundedGLExecutor& gl_executor;
		std::unordered_map<ChunkPosition, std::reference_wrapper<Mesh>>& mesh_position_map;

		void consume_vertices(std::pair<ChunkPosition, std::vector<Vertex>>&& pos_vertices) override;
	};

	int view_distance;
	TimeBoundedGLExecutor& gl_executor;
	std::optional<ChunkPosition> current_center;
	ChunkSurrounder chunk_surrounder;
	const World& world;
	VertexBuilder<Vertex>& update_vertex_builder;
	UpdateVertexConsumer update_vertex_consumer;

	std::vector<Mesh> meshes;

	multimap3d<int16_t, MeshWrapper> mesh_cleanup_map;
	std::unordered_map<ChunkPosition, std::reference_wrapper<Mesh>> mesh_position_map;
	std::vector<std::reference_wrapper<Mesh>> free_meshes;

	void on_center_changed(const ChunkPosition& center) override;
	void consume_vertices(std::pair<ChunkPosition, std::vector<Vertex>>&& pos_vertices) override;

	template <size_t D>
	void free_out_of_bounds_meshes_dim(const ChunkPosition& center);
};


#endif //VOXELGAME_OPENGL_QT_CHUNK_MESH_MANAGER_H
