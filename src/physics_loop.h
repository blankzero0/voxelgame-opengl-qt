#ifndef VOXELGAME_OPENGL_QT_PHYSICS_LOOP_H
#define VOXELGAME_OPENGL_QT_PHYSICS_LOOP_H


#include <thread>
#include "player.h"
#include "input.h"
#include "world.h"

class PhysicsLoop {
public:
	explicit PhysicsLoop(Player& player, const Input& input, World& world);
private:
	Player& player;
	const Input& input;
	World& world;
	std::jthread thread;

	float place_cooldown = 0;
	float destroy_cooldown = 0;

	void run(const std::stop_token& stop_token);
	void tick(float delta_time);
};


#endif //VOXELGAME_OPENGL_QT_PHYSICS_LOOP_H
