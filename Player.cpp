#include "Player.hpp"

#include <vector>

Player::Player(Scene::Transform *cat_head, Scene::Transform* cat_tail, glm::vec2 head_pos_, glm::vec2 tail_pos_, glm::vec2 head_vel_, glm::vec2 tail_vel_)
	: head_pos{head_pos_}, tail_pos{tail_pos_}, head_vel{head_vel_}, tail_vel{tail_vel_}
{
	head = cat_head;
	tail = cat_tail;
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

/* Checks to see if the player has approached any grab points, and automatically grabs 
   onto that point if they have */
void Player::do_auto_grab(std::vector<Grab_Point> &grab_points) {
	for(Grab_Point &p : grab_points) {
		float dist = glm::distance(p.position, head_pos);

		// Only perform a grab upon entry into the grab radius
		if(dist <= grab_radius && p.past_player_dist > grab_radius) {
			fixed_head = true;
		}

		p.past_player_dist = dist;
	}
}

void Player::movement(float elapsed, bool left, bool right, bool up) {
	// If the head is grabbing something, or we are on the ground, stay still
	if(grabbing || head_grounded) {
		head_vel.x = 0.f;
		head_vel.y = 0.f;
	}

	// If the tail is grounded and being controlled, just stay still
	if(grabbing && tail_grounded) {
		tail_vel.x = 0.f;
		tail_vel.y = 0.f;
	}
	
	// Control the head
	if(!grabbing) {
		if (left) head_vel.x = -speed;
		else if (right) head_vel.x = speed;
		if (up && head_grounded) 
		{	
			head_vel.y = jump_speed;
			tail_vel.y += jump_speed / 2.f;
		}
	}
	// Control the tail
	else {
		if (left) tail_vel.x = -speed;
		else if (right) tail_vel.x = speed;
		if (up && tail_grounded) tail_vel.y = jump_speed;
	}

	// Compute spring force on the tail
	glm::vec2 disp = (head_pos - tail_pos);
	float dist = std::max(0.f, glm::distance(head_pos, tail_pos) - length);
	if (disp != glm::vec2(0.f)) {
		glm::vec2 spring_force = glm::normalize(disp) * dist * k;
		tail_vel += spring_force * elapsed;
	}

	// Limit distance between the head and tail
	if(glm::distance(head_pos, tail_pos) > bound_length) {
		glm::vec2 disp = tail_pos - head_pos;

		tail_pos = head_pos + (glm::normalize(disp) * bound_length);
	}
}

void Player::turn_cat() {
	/*
	if (!player.fixed_head) {
		if (player.head_vel.x > 0.f && direction) { //using direction to avoid unnecessary writes to rotation
			head->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
			direction = 0;
		}
		else if (player.head_vel.x < 0.f && !direction) {
			head->rotation = glm::quat(0.0f, 0.0f, 1.0f, 0.0f);
			direction = 1;
		}
	}
	if (player.tail_vel.x > 0.f && tail_direction) {
		cat_tail->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		tail_direction = 0;
	}
	else if (player.tail_vel.x < 0.f && !tail_direction) {
		cat_tail->rotation = glm::quat(0.0f, 0.0f, 1.0f, 0.0f);
		tail_direction = 1;
	}
	*/

	glm::vec3 disp = head->position - tail->position;
	glm::vec3 up(0.f, 1.f, 0.f);

	head->rotation = glm::quatLookAt(glm::normalize(disp), up) * glm::quat(up * (3.14159f / 2));
	tail->rotation = glm::quatLookAt(glm::normalize(disp), up) * glm::quat(up * (3.14159f / 2));
}

void Player::phys_update(float elapsed, const CollisionManager &cm, bool left, bool right, bool up, bool space, std::vector<Grab_Point> &grab_points, float &timer, float fixed_time) {
	// Apply gravity
	head_vel.y -= elapsed * GRAVITY;
	tail_vel.y -= elapsed * GRAVITY;
	for (auto b : body) {
		b.vel -= elapsed * GRAVITY;
	}
	
	length = space ? max_length : 1.f;

	float head_tail_dist = glm::distance(head_pos, tail_pos);

	if (space) {
		if(head_tail_dist > 4.f)
			stretched = true;
	}
	// If not pressing stretch, grabbing onto something, and the player has just recompressed, 
	// let go and apply the velocity to both halves
	else if (fixed_head && stretched && head_tail_dist <= 4.f) {
		stretched = false;

		fixed_head = false;
		head_grounded = false;
		head_vel += tail_vel *0.5f;
		tail_vel *= 0.5f;

		printf("Recompressed %f\n", head_vel.x);
	}

	do_auto_grab(grab_points);

	if (!fixed_head && !fixed_tail) {
		free_movement(elapsed, left, right, up);
	} else if (fixed_head && fixed_tail) {
		head_vel.x = 0.f;
		head_vel.y = 0.f;
		tail_vel.x = 0.f;
		tail_vel.y = 0.f;
		//std::cout << "you have stuck both your head and tail and cannot move\n";
	} else if (fixed_head) {
		fixed_head_movement(elapsed, left, right, up);
	}

	// Do physics update
	head_pos += head_vel * elapsed;
	tail_pos += tail_vel * elapsed;

	// Check collision with the walls and adjust the velocities accordingly
	collide_segments(cm, 1.f, true);
	collide_segments(cm, 1.f, false);

	// Air resistance only FIXED UPDATE
	timer += elapsed;
	while (timer > fixed_time) {
		head_vel *= 0.995f;
		tail_vel *= 0.995f;
		timer -= fixed_time;
	}
}

void Player::respawn() {
	head_pos = head_respawn_pos;
	head_vel = glm::vec2(0.f);
	tail_pos = head_respawn_pos;
	tail_pos.x -= 1.f; //to give space between head and tail
	tail_vel = glm::vec2(0.f);
}