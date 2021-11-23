#pragma once

#include <glm/glm.hpp>

#include "Collisions.hpp"

struct Player {

	Player() = default;
	Player(glm::vec2 head_pos_, glm::vec2 tail_pos_, glm::vec2 head_vel_, glm::vec2 tail_vel_);

	float speed = 10.f;
	float jump_speed = 10.f;
	float grab_radius = 2.f;

	float length = 1.f; 	   // The length of the spring
	float max_length = 10.f;   // The longest spring length the player can stretch to
	float bound_length = 1.f;  // The length which the player cannot stretch beyond
	float k = 20.f; //spring constant

	glm::vec2 head_pos, tail_pos;
	glm::vec2 head_vel, tail_vel;
	glm::vec2 head_respawn_pos, tail_respawn_pos;

	// While older versions let us do both, now you can only grab with your head
	bool grabbing = false;

	bool head_grounded = false;
	bool tail_grounded = false;

	void collide_segments(const CollisionManager &cm, float radius, bool is_head);
	void movement(float elapsed, bool left, bool right, bool up);
	void respawn();
};