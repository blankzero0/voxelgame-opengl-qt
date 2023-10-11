#ifndef VOXELGAME_OPENGL_QT_WORLD_RENDERER_H
#define VOXELGAME_OPENGL_QT_WORLD_RENDERER_H


#include <concepts>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include "block_mesh.h"
#include "world.h"
#include "synchronized_queue.h"
#include "chunk_surrounder.h"
#include "multimap3d.h"
#include "block_mesh_vertex_builder.h"
#include "chunk_mesh_manager.h"
#include "water_mesh.h"
#include "water_mesh_vertex_builder.h"
#include "player.h"

class WorldRenderer : private QOpenGLFunctions_4_5_Core {
public:
	WorldRenderer(World& world, Player& player, TimeBoundedGLExecutor& gl_executor);
	void render();
	void set_view_projection(const QMatrix4x4& view_projection);
	void set_center(const Point& center_point);

private:
	static constexpr int view_distance = 8;

	World& world;
	QOpenGLTexture texture;
	QOpenGLBuffer element_buffer;
	QOpenGLShaderProgram chunk_shader;

	QOpenGLVertexArrayObject blocks_vao;
	BlockMeshVertexBuilder block_vertex_builder;
	BlockMeshVertexBuilder updating_block_vertex_builder;
	BlockMeshVertexBuilder::BlockUpdateConnector block_update_connector;
	ChunkMeshManager<BlockMesh, BlockVertex> block_chunk_mesh_manager;

	QOpenGLShaderProgram water_shader;
	QOpenGLVertexArrayObject water_vao;
	WaterMeshVertexBuilder water_vertex_builder;
	WaterMeshVertexBuilder updating_water_vertex_builder;
	WaterMeshVertexBuilder::WaterUpdateConnector water_update_connector;
	ChunkMeshManager<WaterMesh, WaterVertex> water_chunk_mesh_manager;
};


#endif //VOXELGAME_OPENGL_QT_WORLD_RENDERER_H
