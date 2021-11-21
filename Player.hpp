#pragma once

#include <glm/glm.hpp>

#include "Collisions.hpp"

struct Player {

	Player() = default;
	Player(glm::vec2 head_pos_, glm::vec2 tail_pos_, glm::vec2 head_vel_, glm::vec2 tail_vel_);

	float speed = 10.f;
	float jump_speed = 10.f;
	float grab_radius = 1.5f;

	float length = 5.f; //length of spring
	float max_length = 10.f; //maximum length
	float k = 20.f; //spring constant

	glm::vec2 head_pos, tail_pos;
	glm::vec2 head_vel, tail_vel;
	glm::vec2 head_respawn_pos, tail_respawn_pos;

	bool head_grounded = false;
	bool tail_grounded = false;

	void collide_segments(const CollisionManager &cm, float radius, bool is_head);
	bool grab_ledge(const CollisionManager &cm, glm::vec2& pos, float radius);
	void free_movement(float elapsed, bool left, bool right, bool up);
	void respawn();
};