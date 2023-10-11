#include "chunk_mesh_manager.h"

template<typename Mesh, typename Vertex>
ChunkMeshManager<Mesh, Vertex>::ChunkMeshManager(size_t view_distance, TimeBoundedGLExecutor& gl_executor, VertexBuilder<Vertex>& vertex_builder, VertexBuilder<Vertex>& update_vertex_builder, UpdateConnector& update_connector)
		: view_distance(view_distance), gl_executor(gl_executor), chunk_surrounder(view_distance, vertex_builder, *this), update_connector(update_connector), update_vertex_builder(update_vertex_builder), update_vertex_consumer{gl_executor, mesh_position_map}
{
	meshes.resize((view_distance * 2 + 1) * (view_distance * 2 + 1) * (view_distance * 2 + 1));

	for (auto& mesh: meshes) {
		free_meshes.emplace_back(mesh);
	}

	vertex_builder.set_vertex_consumer(*this);
	update_vertex_builder.set_vertex_consumer(update_vertex_consumer);
}

template<typename Mesh, typename Vertex>
void ChunkMeshManager<Mesh, Vertex>::set_center(const Point& center_point)
{
	chunk_surrounder.set_center(center_point);
}

template<typename Mesh, typename Vertex>
std::vector<Mesh>& ChunkMeshManager<Mesh, Vertex>::get_meshes()
{
	return meshes;
}

template<typename Mesh, typename Vertex>
template<size_t D>
void ChunkMeshManager<Mesh, Vertex>::free_out_of_bounds_meshes_dim(const ChunkPosition& center)
{
	auto first_low = mesh_cleanup_map.template begin<D>();
	auto last_low = mesh_cleanup_map.template lower_bound<D>(center.get<D>() - view_distance);
	std::for_each(first_low, last_low, [](auto& v) { v.second.value.free(); });
	mesh_cleanup_map.template erase<D>(first_low, last_low);

	auto first_high = mesh_cleanup_map.template upper_bound<D>(center.get<D>() + view_distance);
	auto last_high = mesh_cleanup_map.template end<D>();
	std::for_each(first_high, last_high, [](auto& v) { v.second.value.free(); });
	mesh_cleanup_map.template erase<D>(first_high, last_high);
}

template<typename Mesh, typename Vertex>
void ChunkMeshManager<Mesh, Vertex>::on_center_changed(const ChunkPosition& center)
{
	current_center = center;
	free_out_of_bounds_meshes_dim<0>(center);
	free_out_of_bounds_meshes_dim<1>(center);
	free_out_of_bounds_meshes_dim<2>(center);
}

template<typename Mesh, typename Vertex>
void ChunkMeshManager<Mesh, Vertex>::consume_vertices(std::pair<ChunkPosition, std::vector<Vertex>>&& pos_vertices)
{
	gl_executor.add_task([this, pos = std::move(pos_vertices.first), vertices = std::move(pos_vertices.second)](){
		// TODO: thread safety

		assert(current_center.has_value());
		if (pos.x < current_center->x - view_distance || pos.x > current_center->x + view_distance ||
				pos.y < current_center->y - view_distance || pos.y > current_center->y + view_distance ||
				pos.z < current_center->z - view_distance || pos.z > current_center->z + view_distance) {
			return;
		}

		if (mesh_position_map.contains(pos)) {
			return;
		}

		assert(!free_meshes.empty());
		Mesh& mesh = free_meshes.back();
		free_meshes.pop_back();
		mesh.update(vertices);

		mesh_position_map.insert({pos, mesh});

		update_connector.connect(pos);

		mesh_cleanup_map.insert({std::array{pos.x, pos.y, pos.z}, MeshWrapper{pos, mesh, update_connector, free_meshes, mesh_position_map}});
	});
}

template<typename Mesh, typename Vertex>
void ChunkMeshManager<Mesh, Vertex>::MeshWrapper::free()
{
	mesh_position_map.erase(position);
	update_connector.disconnect(position);
	mesh.clear();
	free_list.emplace_back(mesh);
}

template<typename Mesh, typename Vertex>
void ChunkMeshManager<Mesh, Vertex>::UpdateVertexConsumer::consume_vertices(std::pair<ChunkPosition, std::vector<Vertex>>&& pos_vertices)
{
	gl_executor.add_task([this, pos = std::move(pos_vertices.first), vertices = std::move(pos_vertices.second)](){
		// TODO: thread safety

		auto it = mesh_position_map.find(pos);
		if (it == mesh_position_map.end())
			return;
		it->second.get().update(vertices);
	});
}

#include "block_mesh.h"
#include "block_vertex.h"
#include "water_mesh.h"
#include "water_vertex.h"
template class ChunkMeshManager<BlockMesh, BlockVertex>;
template class ChunkMeshManager<WaterMesh, WaterVertex>;
