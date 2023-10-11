#include "water_mesh.h"
#include "block_vertex.h"
#include "water_vertex.h"

WaterMesh::WaterMesh()
		: index_count(0)
{
	QOpenGLFunctions_4_5_Core::initializeOpenGLFunctions();

	glGenBuffers(1, &vertex_buffer_id);
}

WaterMesh::WaterMesh(WaterMesh&& other) noexcept
		: index_count(std::exchange(other.index_count, 0)), vertex_buffer_id(std::exchange(other.vertex_buffer_id, 0))
{
	QOpenGLFunctions_4_5_Core::initializeOpenGLFunctions();
}

void WaterMesh::draw()
{
	glBindVertexBuffer(0, vertex_buffer_id, 0, sizeof(WaterVertex));
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, static_cast<void*>(0));
}

void WaterMesh::update(const std::vector<WaterVertex>& vertices)
{
	size_t vertex_count = vertices.size();
	index_count = static_cast<GLsizei>(vertex_count / 4 * 6);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(WaterVertex) * vertex_count, vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void WaterMesh::clear()
{
	index_count = 0;
}

WaterMesh::~WaterMesh()
{
	glDeleteBuffers(1, &vertex_buffer_id);
}
