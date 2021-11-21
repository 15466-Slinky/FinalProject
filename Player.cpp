#include "Player.hpp"

#include <vector>

void Player::collide_segments(CollisionManager &cm, glm::vec2 &pos, glm::vec2 &vel, float radius, bool &grounded) {
	grounded = false;

	for(CollisionManager::line_segment &ls : cm.line_segments) {
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
}