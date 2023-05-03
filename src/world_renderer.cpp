#include "world_renderer.h"

#include <cmath>
#include <unordered_set>
#include "chunk_mesh_vertex_builder.h"


WorldRenderer::WorldRenderer(World& world, ChunkMeshVertexBuilder& chunk_mesh_vertex_builder)
		: texture(QOpenGLTexture::Target2DArray), world(world), chunk_mesh_vertex_builder(chunk_mesh_vertex_builder), chunk_surrounder(chunk_mesh_vertex_builder, *this),
		chunk_meshes(std::make_unique<decltype(chunk_meshes)::element_type>()), updating_vertex_builder(world)
{
	QOpenGLFunctions_4_5_Core::initializeOpenGLFunctions();

	if (!chunk_shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/chunk/vertex.glsl") ||
			!chunk_shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/chunk/fragment.glsl") ||
			!chunk_shader.link()) {
		throw std::runtime_error(chunk_shader.log().toStdString());
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

	vertex_array_object.create();

	vertex_array_object.bind();
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
	glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texture_coords));
	glVertexAttribIFormat(2, 1, GL_BYTE, offsetof(Vertex, texture_index));
	glVertexAttribFormat(3, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
	glVertexAttribBinding(0, 0);
	glVertexAttribBinding(1, 0);
	glVertexAttribBinding(2, 0);
	glVertexAttribBinding(3, 0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	vertex_array_object.release();

	for (auto& mesh: *chunk_meshes) {
		free_meshes.emplace_back(mesh);
	}

	updating_vertex_builder.set_vertex_consumer(
			[this](std::pair<ChunkPosition, std::vector<Vertex>>&& p) {
				auto it = mesh_position_map.find(p.first);
				if (it == mesh_position_map.end())
					return;

				pending_updated_mesh_vertices.put(std::move(p));
			}
	);
}

void WorldRenderer::render()
{
	auto render_start = std::chrono::steady_clock::now();
	std::chrono::duration<unsigned int, std::ratio<1, 60 * 2>> max_upload_time(1); // half a frame

	while ((std::chrono::steady_clock::now() - render_start) < max_upload_time) {
		std::optional mesh_vertices_opt = pending_updated_mesh_vertices.take();
		if (!mesh_vertices_opt) break;
		const auto& [pos, vertices] = *mesh_vertices_opt;

		auto it = mesh_position_map.find(pos);
		if (it == mesh_position_map.end())
			return;
		it->second.get().update(vertices);
	}

	while ((std::chrono::steady_clock::now() - render_start) < max_upload_time) {
		std::optional mesh_vertices = pending_mesh_vertices.take();
		if (!mesh_vertices) break;
		ChunkPosition& pos = mesh_vertices->first;

		assert(current_center.has_value());
		if (pos.x < current_center->x - view_distance || pos.x > current_center->x + view_distance ||
				pos.y < current_center->y - view_distance || pos.y > current_center->y + view_distance ||
				pos.z < current_center->z - view_distance || pos.z > current_center->z + view_distance)
			continue;

		if (mesh_position_map.contains(pos))
			continue;

		assert(!free_meshes.empty());
		ChunkMesh& mesh = free_meshes.back();
		free_meshes.pop_back();
		mesh.update(mesh_vertices->second);

		const Chunk* c = world.chunk_at(pos);
		assert(c);
		const Chunk& chunk = *c;

		mesh_position_map.insert({pos, mesh});

		mesh_3d_map.insert({std::array{pos.x, pos.y, pos.z}, MeshWrapper{pos, mesh, free_meshes, chunk, mesh_position_map}});
		chunk.set_changed_listener(
				[this, pos, &chunk]() {
					updating_vertex_builder.supply_chunk(pos, chunk);
				}
		);
	}

	texture.bind();

	chunk_shader.bind();

	vertex_array_object.bind();
	for (auto& renderer: *chunk_meshes) {
		renderer.draw();
	}
	vertex_array_object.release();

	chunk_shader.release();

	texture.release();
}

void WorldRenderer::set_view_projection(const QMatrix4x4& view_projection)
{
	chunk_shader.bind();
	chunk_shader.setUniformValue("mvp", view_projection);
	chunk_shader.release();
}

template<size_t D>
void WorldRenderer::free_out_of_bounds_meshes_dim(const ChunkPosition& center)
{
	auto first_low = mesh_3d_map.begin<D>();
	auto last_low = mesh_3d_map.lower_bound<D>(center.get<D>() - view_distance);
	std::for_each(first_low, last_low, [](auto& v) { v.second.value.free(); });
	mesh_3d_map.erase<D>(first_low, last_low);

	auto first_high = mesh_3d_map.upper_bound<D>(center.get<D>() + view_distance);
	auto last_high = mesh_3d_map.end<D>();
	std::for_each(first_high, last_high, [](auto& v) { v.second.value.free(); });
	mesh_3d_map.erase<D>(first_high, last_high);
}

void WorldRenderer::on_center_changed(const ChunkPosition& center)
{
	current_center = center;
	free_out_of_bounds_meshes_dim<0>(center);
	free_out_of_bounds_meshes_dim<1>(center);
	free_out_of_bounds_meshes_dim<2>(center);
}

void WorldRenderer::set_center(const Point& center_point)
{
	chunk_surrounder.set_center(center_point);
}

void WorldRenderer::submit_mesh_vertices(std::pair<ChunkPosition, std::vector<Vertex>>&& mesh_vertices)
{
	pending_mesh_vertices.put(std::move(mesh_vertices));
}

void WorldRenderer::MeshWrapper::free()
{
	mesh_position_map.erase(position);
	chunk.clear_changed_listener();
	mesh.clear();
	free_list.emplace_back(mesh);
}
