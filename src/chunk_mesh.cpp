#include "chunk_mesh.h"
#include "vertex.h"

ChunkMesh::ChunkMesh()
		: vertex_count(0)
{
	QOpenGLFunctions_4_5_Core::initializeOpenGLFunctions();

	glGenBuffers(1, &vertex_buffer_id);
}

ChunkMesh::ChunkMesh(ChunkMesh&& other) noexcept
		: vertex_count(std::exchange(other.vertex_count, 0)), vertex_buffer_id(std::exchange(other.vertex_buffer_id, 0))
{
	QOpenGLFunctions_4_5_Core::initializeOpenGLFunctions();
}

void ChunkMesh::draw()
{
	glBindVertexBuffer(0, vertex_buffer_id, 0, sizeof(Vertex));
	glDrawArrays(GL_TRIANGLES, 0, vertex_count);
}

void ChunkMesh::update(const std::vector<Vertex>& vertices)
{
	vertex_count = static_cast<GLsizei>(vertices.size());

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertex_count, vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ChunkMesh::clear()
{
	vertex_count = 0;
}

ChunkMesh::~ChunkMesh()
{
	glDeleteBuffers(1, &vertex_buffer_id);
}
