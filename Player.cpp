#include "Player.hpp"

#include <vector>

Player::Player(glm::vec2 head_pos_, glm::vec2 tail_pos_, glm::vec2 head_vel_, glm::vec2 tail_vel_)
	: head_pos{head_pos_}, tail_pos{tail_pos_}, head_vel{head_vel_}, tail_vel{tail_vel_}
{
	head_respawn_pos = head_pos;
	tail_respawn_pos = tail_pos;
}

void Player::collide_segments(const CollisionManager &cm, float radius, bool is_head) {
	glm::vec2 pos;
	glm::vec2 vel;
	bool grounded = false;

	if (is_head) {
		pos = head_pos;
		vel = head_vel;
	} else {
		pos = tail_pos;
		vel = tail_vel;
	}

	for(const CollisionManager::line_segment &ls : cm.line_segments) {
		CollisionManager::circle c(pos, radius);

		bool is_hit = false;
		CollisionManager::intersection hit = cm.get_capsule_collision(c, ls, is_hit);

		if(is_hit) {
			pos = hit.point_of_intersection;

			// Project the velocity onto the normal, and subtract that component so
			// vel is perpendicular
			float d = glm::dot(hit.surface_normal, vel);
			if (d > 0.f) d = 0.f;
			vel -= hit.surface_normal * d;

			if(hit.surface_normal.y > 0.5f) {
				grounded = true;
			}
		}

	}

	if (is_head) {
		head_pos = pos;
		head_vel = vel;
		head_grounded = grounded;
	} else {
		tail_pos = pos;
		tail_vel = vel;
		tail_grounded = grounded;
	}
}

bool Player::grab_ledge(const CollisionManager &cm, glm::vec2& pos, float radius) {
	CollisionManager::circle c(pos, radius);
	std::vector<CollisionManager::intersection> hits = cm.get_collisions_all(c);
	return (!hits.empty());
}

void Player::free_movement(float elapsed, bool left, bool right, bool up) {
	// If the head is grounded, just stay still
	if(head_grounded) {
		head_vel.x = 0.f;
		head_vel.y = 0.f;
	}

	if (left) head_vel.x = -speed;
	else if (right) head_vel.x = speed;
	if (up && head_grounded) 
	{	
		head_vel.y = jump_speed;
		tail_vel.y += jump_speed / 2.f;
	}

	glm::vec2 disp = (head_pos - tail_pos);
	float dist = std::max(0.f, glm::distance(head_pos, tail_pos) - length);
	if (disp != glm::vec2(0.f)) {
		glm::vec2 spring_force = glm::normalize(disp) * dist * k;
		tail_vel += spring_force * elapsed;
	}
	
	// Only pull on the head if it's not standing somewhere
	//if(!head_grounded)
	//	head_vel -= spring_force * elapsed;
}

void Player::respawn() {
	head_pos = head_respawn_pos;
	head_vel = glm::vec2(0.f);
	tail_pos = head_respawn_pos;
	tail_pos.x -= 1.f; //to give space between head and tail
	tail_vel = glm::vec2(0.f);
}