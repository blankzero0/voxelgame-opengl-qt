#ifndef VOXELGAME_OPENGL_QT_CHUNK_MESH_H
#define VOXELGAME_OPENGL_QT_CHUNK_MESH_H


#include <functional>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_4_5_Core>
#include "chunk.h"
#include "vertex.h"

class ChunkMesh : private QOpenGLFunctions_4_5_Core {
public:
	ChunkMesh();
	ChunkMesh(ChunkMesh&&) noexcept;
	ChunkMesh(ChunkMesh&) = delete;
	ChunkMesh& operator=(ChunkMesh&) = delete;
	ChunkMesh& operator=(ChunkMesh&&) = delete;
	~ChunkMesh() override;
	// vao has to be bound
	void draw();
	void update(const std::vector<Vertex>& vertices);
	void clear();

private:
	GLuint vertex_buffer_id;
	GLsizei index_count;
};


#endif //VOXELGAME_OPENGL_QT_CHUNK_MESH_H
