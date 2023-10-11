#ifndef VOXELGAME_OPENGL_QT_WATER_MESH_H
#define VOXELGAME_OPENGL_QT_WATER_MESH_H


#include <functional>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_4_5_Core>
#include "block_vertex.h"
#include "water_vertex.h"

class WaterMesh : private QOpenGLFunctions_4_5_Core {
public:
	WaterMesh();
	WaterMesh(WaterMesh&&) noexcept;
	WaterMesh(WaterMesh&) = delete;
	WaterMesh& operator=(WaterMesh&) = delete;
	WaterMesh& operator=(WaterMesh&&) = delete;
	~WaterMesh() override;
	// vao has to be bound
	void draw();
	void update(const std::vector<WaterVertex>& vertices);
	void clear();

private:
	GLuint vertex_buffer_id;
	GLsizei index_count;
};


#endif //VOXELGAME_OPENGL_QT_WATER_MESH_H
