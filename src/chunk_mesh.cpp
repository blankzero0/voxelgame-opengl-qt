#include "chunk_mesh.h"
#include "vertex.h"

ChunkMesh::ChunkMesh()
		: index_count(0)
{
	QOpenGLFunctions_4_5_Core::initializeOpenGLFunctions();

	glGenBuffers(1, &vertex_buffer_id);
}

ChunkMesh::ChunkMesh(ChunkMesh&& other) noexcept
		: index_count(std::exchange(other.index_count, 0)), vertex_buffer_id(std::exchange(other.vertex_buffer_id, 0))
{
	QOpenGLFunctions_4_5_Core::initializeOpenGLFunctions();
}

void ChunkMesh::draw()
{
	glBindVertexBuffer(0, vertex_buffer_id, 0, sizeof(Vertex));
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, static_cast<void*>(0));
}

void ChunkMesh::update(const std::vector<Vertex>& vertices)
{
	size_t vertex_count = vertices.size();
	index_count = static_cast<GLsizei>(vertex_count / 4 * 6);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertex_count, vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ChunkMesh::clear()
{
	index_count = 0;
}

ChunkMesh::~ChunkMesh()
{
	glDeleteBuffers(1, &vertex_buffer_id);
}
