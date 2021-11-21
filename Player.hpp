#pragma once

#include <glm/glm.hpp>

#include "Collisions.hpp"

struct Player {

	Player() = default;
	Player(glm::vec2 head_pos_, glm::vec2 tail_pos_)
		: head_pos{head_pos_}, tail_pos{tail_pos_} {}

	float speed = 10.f;
	float jump_speed = 10.f;
	float grab_radius = 1.5f;

	glm::vec2 head_pos, tail_pos;
	glm::vec2 head_vel, tail_vel;

	bool head_grounded = false;
	bool tail_grounded = false;

	void collide_segments(CollisionManager &cm, float radius, bool is_head);
};