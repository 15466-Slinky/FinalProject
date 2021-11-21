#pragma once

#include <glm/glm.hpp>

#include "Collisions.hpp"
#include "KeyPoint.hpp"
#include "Scene.hpp"

struct Player {

	Player() = default;
	Player(Scene::Transform *cat_head, Scene::Transform *cat_tail, glm::vec2 head_pos_, glm::vec2 tail_pos_, glm::vec2 head_vel_, glm::vec2 tail_vel_);

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
	bool fixed_head = false;
	bool fixed_tail = false;

	Scene::Transform* head = nullptr;
	Scene::Transform* tail = nullptr;

	void collide_segments(const CollisionManager &cm, float radius, bool is_head);
	bool grab_ledge(const CollisionManager &cm, glm::vec2& pos, float radius);
	void do_auto_grab(std::vector<Grab_Point> &grab_points);
	void free_movement(float elapsed, bool left, bool right, bool up);
	void fixed_head_movement(float elapsed, bool left, bool right, bool up);
	void fixed_tail_movement(float elapsed, bool left, bool right, bool up);
	void turn_cat();
	void respawn();
};