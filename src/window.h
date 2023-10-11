#ifndef VOXELGAME_OPENGL_QT_WINDOW_H
#define VOXELGAME_OPENGL_QT_WINDOW_H


#include <QOpenGLWindow>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLDebugLogger>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <vector>
#include "player.h"
#include "input.h"
#include "world_renderer.h"
#include "selection_renderer.h"
#include "block_mesh_vertex_builder.h"

class Window : public QOpenGLWindow, private QOpenGLFunctions_4_5_Core {
public:
	Window(World& world, Player& player, Input& input);
	~Window() override;

protected:
	void initializeGL() override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
private:
	World& world;
	Player& player;
	Input& input;
	TimeBoundedGLExecutor time_bounded_gl_executor;
	QOpenGLDebugLogger logger;
	QMatrix4x4 projection_matrix;
	std::optional<WorldRenderer> world_renderer;
	std::optional<BlockMeshVertexBuilder> chunk_mesh_vertex_builder;
	std::optional<SelectionRenderer> selection_renderer;
	std::chrono::time_point<std::chrono::steady_clock> last_render;
};


#endif //VOXELGAME_OPENGL_QT_WINDOW_H
