#include "world_renderer.h"

#include <unordered_set>
#include "block_mesh_vertex_builder.h"
#include "water_vertex.h"


WorldRenderer::WorldRenderer(World& world, TimeBoundedGLExecutor& gl_executor)
		: texture(QOpenGLTexture::Target2DArray), world(world), element_buffer(QOpenGLBuffer::IndexBuffer), block_vertex_builder(world), updating_block_vertex_builder(world),
		block_chunk_mesh_manager(view_distance, gl_executor, block_vertex_builder, updating_block_vertex_builder, world)
{
	QOpenGLFunctions_4_5_Core::initializeOpenGLFunctions();

	if (!chunk_shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/chunk/vertex.glsl") ||
			!chunk_shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/chunk/fragment.glsl") ||
			!chunk_shader.link()) {
		throw std::runtime_error(chunk_shader.log().toStdString());
	}

	if (!water_shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/water/vertex.glsl") ||
			!water_shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/water/fragment.glsl") ||
			!water_shader.link()) {
		throw std::runtime_error(water_shader.log().toStdString());
	}

	std::array texture_associations = std::to_array<std::pair<int, const char*>>(
			{
					{0, ":/textures/grass.png"},
					{1, ":/textures/grass_side.png"},
					{2, ":/textures/dirt.png"},
					{3, ":/textures/stone.png"},
					{4, ":/textures/cobblestone.png"},
					{5, ":/textures/sand.png"},
					{6, ":/textures/water.png"},
			}
	);

	glActiveTexture(GL_TEXTURE0);
	texture.create();
	texture.setFormat(QOpenGLTexture::TextureFormat::RGB8_UNorm);
	texture.setSize(128, 128);
	texture.setLayers(texture_associations.size());
	texture.setMipLevels(texture.maximumMipLevels());
	float anisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &anisotropy);
	texture.setMaximumAnisotropy(anisotropy);
	texture.allocateStorage();

	for (auto [index, path]: texture_associations) {
		QImage image(path);
		image.mirror();
		texture.setData(0, index, QOpenGLTexture::PixelFormat::BGRA, QOpenGLTexture::PixelType::UInt8, image.bits());
	}

	texture.setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	texture.setMagnificationFilter(QOpenGLTexture::Nearest);
	texture.setWrapMode(QOpenGLTexture::ClampToEdge);
	texture.generateMipMaps();

	blocks_vao.create();

	{
		constexpr size_t max_face_count = Chunk::size * Chunk::size * Chunk::size * 6;
		constexpr size_t max_index_count = max_face_count * 6;
		std::vector<uint32_t> indices;
		indices.reserve(max_index_count);
		for (size_t i = 0, j = 0; i < max_face_count; i++, j+=4) {
			indices.push_back(j+0);
			indices.push_back(j+1);
			indices.push_back(j+2);
			indices.push_back(j+2);
			indices.push_back(j+3);
			indices.push_back(j+0);
		}
		element_buffer.create();
		element_buffer.bind();
		element_buffer.allocate(indices.data(), indices.size() * sizeof(decltype(indices)::value_type));
		element_buffer.release();
	}

	blocks_vao.bind();
	element_buffer.bind();
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(BlockVertex, position));
	glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(BlockVertex, texture_coords));
	glVertexAttribIFormat(2, 1, GL_BYTE, offsetof(BlockVertex, texture_index));
	glVertexAttribFormat(3, 3, GL_FLOAT, GL_FALSE, offsetof(BlockVertex, normal));
	glVertexAttribBinding(0, 0);
	glVertexAttribBinding(1, 0);
	glVertexAttribBinding(2, 0);
	glVertexAttribBinding(3, 0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	blocks_vao.release();
}

void WorldRenderer::render()
{
	texture.bind();

	chunk_shader.bind();

	blocks_vao.bind();
	for (auto& renderer: block_chunk_mesh_manager.get_meshes()) {
		renderer.draw();
	}
	blocks_vao.release();

	chunk_shader.release();

	texture.release();
}

void WorldRenderer::set_view_projection(const QMatrix4x4& view_projection)
{
	chunk_shader.bind();
	chunk_shader.setUniformValue("mvp", view_projection);
	chunk_shader.release();

	water_shader.bind();
	water_shader.setUniformValue("mvp", view_projection);
	water_shader.release();
}

void WorldRenderer::set_center(const Point& center_point)
{
	block_chunk_mesh_manager.set_center(center_point);
}
