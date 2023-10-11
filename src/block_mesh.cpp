#include "block_mesh.h"
#include "block_vertex.h"

BlockMesh::BlockMesh()
		: index_count(0)
{
	QOpenGLFunctions_4_5_Core::initializeOpenGLFunctions();

	glGenBuffers(1, &vertex_buffer_id);
}

BlockMesh::BlockMesh(BlockMesh&& other) noexcept
		: index_count(std::exchange(other.index_count, 0)), vertex_buffer_id(std::exchange(other.vertex_buffer_id, 0))
{
	QOpenGLFunctions_4_5_Core::initializeOpenGLFunctions();
}

void BlockMesh::draw()
{
	glBindVertexBuffer(0, vertex_buffer_id, 0, sizeof(BlockVertex));
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, static_cast<void*>(0));
}

void BlockMesh::update(const std::vector<BlockVertex>& vertices)
{
	size_t vertex_count = vertices.size();
	index_count = static_cast<GLsizei>(vertex_count / 4 * 6);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BlockVertex) * vertex_count, vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BlockMesh::clear()
{
	index_count = 0;
}

BlockMesh::~BlockMesh()
{
	glDeleteBuffers(1, &vertex_buffer_id);
}
