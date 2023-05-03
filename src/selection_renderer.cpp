#include "selection_renderer.h"
#include <array>

namespace {

std::array vertices = std::to_array<Point>({
		{0, 0, 0},
		{1, 0, 0},
		{0, 1, 0},
		{1, 1, 0},
		{0, 0, 1},
		{1, 0, 1},
		{0, 1, 1},
		{1, 1, 1},

		{0, 0, 0},
		{0, 1, 0},
		{1, 0, 0},
		{1, 1, 0},
		{0, 0, 1},
		{0, 1, 1},
		{1, 0, 1},
		{1, 1, 1},

		{0, 0, 0},
		{0, 0, 1},
		{1, 0, 0},
		{1, 0, 1},
		{0, 1, 0},
		{0, 1, 1},
		{1, 1, 0},
		{1, 1, 1},
});

}

SelectionRenderer::SelectionRenderer()
{
	QOpenGLFunctions_4_5_Core::initializeOpenGLFunctions();

	if (!selection_shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/selection/vertex.glsl") ||
			!selection_shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/selection/fragment.glsl") ||
			!selection_shader.link()) {
		throw std::runtime_error(selection_shader.log().toStdString());
	}

	vertex_buffer.create();
	glGenVertexArrays(1, &vertex_array_object);

	glBindVertexArray(vertex_array_object);
	vertex_buffer.bind();
	vertex_buffer.allocate(vertices.data(), sizeof(Point) * vertices.size());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), nullptr);
	vertex_buffer.release();
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	update_mvp();
}

void SelectionRenderer::render()
{
	selection_shader.bind();

	glBindVertexArray(vertex_array_object);
	glDrawArrays(GL_LINES, 0, vertices.size());
	glBindVertexArray(0);

	selection_shader.release();
}

void SelectionRenderer::set_position(const BlockPosition& position)
{
	model.setToIdentity();
	model.translate(position.x, position.y, position.z);

	update_mvp();
}

void SelectionRenderer::set_view_projection(const QMatrix4x4& view_projection)
{
	this->view_projection = view_projection;
	update_mvp();
}

void SelectionRenderer::update_mvp()
{
	QMatrix4x4 mvp = view_projection * model;
	selection_shader.bind();
	selection_shader.setUniformValue("mvp", mvp);
	selection_shader.release();
}
