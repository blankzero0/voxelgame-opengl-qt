#ifndef VOXELGAME_OPENGL_QT_BLOCK_MESH_H
#define VOXELGAME_OPENGL_QT_BLOCK_MESH_H


#include <functional>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_4_5_Core>
#include "chunk.h"
#include "block_vertex.h"

class BlockMesh : private QOpenGLFunctions_4_5_Core {
public:
	BlockMesh();
	BlockMesh(BlockMesh&&) noexcept;
	BlockMesh(BlockMesh&) = delete;
	BlockMesh& operator=(BlockMesh&) = delete;
	BlockMesh& operator=(BlockMesh&&) = delete;
	~BlockMesh() override;
	// vao has to be bound
	void draw();
	void update(const std::vector<BlockVertex>& vertices);
	void clear();

private:
	GLuint vertex_buffer_id;
	GLsizei index_count;
};


#endif //VOXELGAME_OPENGL_QT_BLOCK_MESH_H
