#include <gtest/gtest.h>

#include "world.h"
#include "chunk.h"
#include "geometry.h"
#include "raycast.h"

TEST(AllTest, EdgeCases) {
	World world_z_0;
	Chunk chunk_z_0;
	chunk_z_0.set_block({0, 0, 0}, player_placed_block);
	chunk_z_0.set_block({1, 0, 0}, player_placed_block);
	world_z_0.add_chunk({0, 0, 0}, std::move(chunk_z_0));

	World world_z_1;
	Chunk chunk_z_1;
	chunk_z_1.set_block({0, 0, 1}, player_placed_block);
	chunk_z_1.set_block({1, 0, 1}, player_placed_block);
	world_z_1.add_chunk({0, 0, 0}, std::move(chunk_z_1));
	
	Point from{1, 0.5, 1};
	Vector direction_x_pos{1, 0, 0};
	Vector direction_x_neg{-1, 0, 0};

	auto result_z_0_x_pos = raycast(world_z_0, from, direction_x_pos, 1);
	auto result_z_0_x_neg = raycast(world_z_0, from, direction_x_neg, 1);
	auto result_z_1_x_pos = raycast(world_z_1, from, direction_x_pos, 1);
	auto result_z_1_x_neg = raycast(world_z_1, from, direction_x_neg, 1);
	
	EXPECT_FALSE(result_z_0_x_pos.has_value());
	EXPECT_FALSE(result_z_0_x_neg.has_value());
	EXPECT_FALSE(result_z_1_x_pos.has_value());
	EXPECT_FALSE(result_z_1_x_neg.has_value());
}

TEST(AllTest, PosX) {
	World world;
	Chunk chunk;
	chunk.set_block({2, 1, 1}, player_placed_block);
	world.add_chunk({0, 0, 0}, std::move(chunk));
	Point from{1.5, 1.5, 1.5};
	Vector direction{1, 0, 0};

	auto result = raycast(world, from, direction, 1);

	EXPECT_TRUE(result.has_value());
}

TEST(AllTest, NegX) {
	World world;
	Chunk chunk;
	chunk.set_block({0, 1, 1}, player_placed_block);
	world.add_chunk({0, 0, 0}, std::move(chunk));
	Point from{1.5, 1.5, 1.5};
	Vector direction{-1, 0, 0};

	auto result = raycast(world, from, direction, 1);

	EXPECT_TRUE(result.has_value());
}

TEST(AllTest, PosY) {
	World world;
	Chunk chunk;
	chunk.set_block({1, 2, 1}, player_placed_block);
	world.add_chunk({0, 0, 0}, std::move(chunk));
	Point from{1.5, 1.5, 1.5};
	Vector direction{0, 1, 0};

	auto result = raycast(world, from, direction, 1);

	EXPECT_TRUE(result.has_value());
}

TEST(AllTest, NegY) {
	World world;
	Chunk chunk;
	chunk.set_block({1, 0, 1}, player_placed_block);
	world.add_chunk({0, 0, 0}, std::move(chunk));
	Point from{1.5, 1.5, 1.5};
	Vector direction{0, -1, 0};

	auto result = raycast(world, from, direction, 1);

	EXPECT_TRUE(result.has_value());
}

TEST(AllTest, PosZ) {
	World world;
	Chunk chunk;
	chunk.set_block({1, 1, 2}, player_placed_block);
	world.add_chunk({0, 0, 0}, std::move(chunk));
	Point from{1.5, 1.5, 1.5};
	Vector direction{0, 0, 1};

	auto result = raycast(world, from, direction, 1);

	EXPECT_TRUE(result.has_value());
}

TEST(AllTest, NegZ) {
	World world;
	Chunk chunk;
	chunk.set_block({1, 1, 0}, player_placed_block);
	world.add_chunk({0, 0, 0}, std::move(chunk));
	Point from{1.5, 1.5, 1.5};
	Vector direction{0, 0, -1};

	auto result = raycast(world, from, direction, 1);

	EXPECT_TRUE(result.has_value());
}

TEST(AllTest, EdgePosX) {
	World world;
	Chunk chunk;
	chunk.set_block({2, 1, 1}, player_placed_block);
	world.add_chunk({0, 0, 0}, std::move(chunk));
	Point from{1, 1.5, 1.5};
	Vector direction{1, 0, 0};

	auto result = raycast(world, from, direction, 1);

	EXPECT_FALSE(result.has_value());
}