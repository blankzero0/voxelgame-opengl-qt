#include <gtest/gtest.h>

#include "world.h"
#include "chunk_mesh_vertex_builder.h"
#include "chunk.h"
#include "geometry.h"

TEST(AllTest, OneBlockVertexCount) {
	World world;
	ChunkMeshVertexBuilder vertex_builder(world);
	ChunkPosition position{0, 99, 0};
	Chunk chunk(position);
	chunk.set_block({0, 0, 0}, player_placed_block);
	vertex_builder.set_vertex_consumer([](std::pair<ChunkPosition, std::vector<Vertex>>&& vertices){
		EXPECT_EQ(vertices.second.size(), 6 * 6);
	});
	vertex_builder.supply_chunk(position, chunk);
}

TEST(AllTest, TwoBlocksVertexCount) {
	World world;
	ChunkMeshVertexBuilder vertex_builder(world);
	ChunkPosition position{0, 99, 0};
	Chunk chunk(position);
	chunk.set_block({0, 0, 0}, player_placed_block);
	chunk.set_block({1, 0, 0}, player_placed_block);
	vertex_builder.set_vertex_consumer([](std::pair<ChunkPosition, std::vector<Vertex>>&& vertices){
		EXPECT_EQ(vertices.second.size(), 2 * 6 * 5);
	});
	vertex_builder.supply_chunk(position, chunk);
}
