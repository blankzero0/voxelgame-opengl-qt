#ifndef VOXELGAME_OPENGL_QT_WORLD_RENDERER_H
#define VOXELGAME_OPENGL_QT_WORLD_RENDERER_H


#include <concepts>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include "chunk_mesh.h"
#include "world.h"
#include "synchronized_queue.h"
#include "chunk_surrounder.h"
#include "multimap3d.h"
#include "chunk_mesh_vertex_builder.h"

class WorldRenderer : public CenterChangeListener<WorldRenderer>, private QOpenGLFunctions_4_5_Core {
public:
	WorldRenderer(World& world, ChunkMeshVertexBuilder& chunk_mesh_vertex_builder);
	void render();
	void set_view_projection(const QMatrix4x4& view_projection);
	void set_center(const Point& center_point);
	void submit_mesh_vertices(std::pair<ChunkPosition, std::vector<Vertex>>&& mesh_vertices);

	void on_center_changed(const ChunkPosition& center);

private:
	static constexpr int view_distance = 8;

	struct MeshWrapper {
		void free();

		ChunkPosition position;
		ChunkMesh& mesh;
		std::vector<std::reference_wrapper<ChunkMesh>>& free_list;
		const Chunk& chunk;
		std::unordered_map<ChunkPosition, std::reference_wrapper<ChunkMesh>>& mesh_position_map;
	};

	World& world;
	ChunkMeshVertexBuilder& chunk_mesh_vertex_builder;
	ChunkMeshVertexBuilder updating_vertex_builder;
	ChunkSurrounder<view_distance, ChunkMeshVertexBuilder, WorldRenderer> chunk_surrounder;
	QOpenGLShaderProgram chunk_shader;
	QOpenGLTexture texture;
	QOpenGLVertexArrayObject vertex_array_object;
	QOpenGLBuffer element_buffer;
	std::unique_ptr<std::array<ChunkMesh, (view_distance * 2 + 1) * (view_distance * 2 + 1) * (view_distance * 2 + 1)>> chunk_meshes;
	multimap3d<int16_t, MeshWrapper> mesh_3d_map;
	std::unordered_map<ChunkPosition, std::reference_wrapper<ChunkMesh>> mesh_position_map;
	std::vector<std::reference_wrapper<ChunkMesh>> free_meshes;
	SynchronizedQueue<std::pair<ChunkPosition, std::vector<Vertex>>> pending_mesh_vertices;
	SynchronizedQueue<std::pair<ChunkPosition, std::vector<Vertex>>> pending_updated_mesh_vertices;
	std::optional<ChunkPosition> current_center;

	template <size_t D>
	void free_out_of_bounds_meshes_dim(const ChunkPosition& center);
};


#endif //VOXELGAME_OPENGL_QT_WORLD_RENDERER_H
