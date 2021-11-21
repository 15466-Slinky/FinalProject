#pragma once

#include <glm/glm.hpp>

#include "Collisions.hpp"

struct Player {

	Player() = default;

	float speed = 10.f;
	float jump_speed = 10.f;
	float grab_radius = 1.5f;

	void collide_segments(CollisionManager &cm, glm::vec2 &pos, glm::vec2 &vel, float radius, bool &grounded);
};