#ifndef VOXELGAME_OPENGL_QT_SELECTION_RENDERER_H
#define VOXELGAME_OPENGL_QT_SELECTION_RENDERER_H


#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLBuffer>
#include "geometry.h"

class SelectionRenderer : private QOpenGLFunctions_4_5_Core {
public:
	SelectionRenderer();

	void render();
	void set_position(const BlockPosition& position);
	void set_view_projection(const QMatrix4x4& view_projection);

private:
	QOpenGLShaderProgram selection_shader;
	GLuint vertex_array_object;
	QOpenGLBuffer vertex_buffer;
	QMatrix4x4 view_projection;
	QMatrix4x4 model;
	void update_mvp();
};


#endif //VOXELGAME_OPENGL_QT_SELECTION_RENDERER_H
