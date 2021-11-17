#include "Collisions.hpp"

#include <glm/gtc/quaternion.hpp>

#include <cmath>

CollisionManager::CollisionManager(std::vector<Scene::Transform*> platforms) {
	for(auto p : platforms) {
		std::vector<CollisionManager::line_segment> lines = get_lines(p);
		assert(lines.size() == 4);
		
		line_segments.emplace_back(lines[0]);	// left
		line_segments.emplace_back(lines[1]);	// right
		line_segments.emplace_back(lines[2]);	// up
		line_segments.emplace_back(lines[3]);	// bottom
	}
}

//returns line segments corresponding to the 4 sides of a platform
std::vector<CollisionManager::line_segment> CollisionManager::get_lines(const Scene::Transform* platform){
	// z component is always 0
	// +y is up, +x is right
	
	glm::vec3 position = platform->position;
	glm::quat rotation = platform->rotation;
	glm::mat3 rotation_matrix = glm::mat3_cast(rotation);
	glm::vec3 scale = platform->scale;
	
	// left line
	glm::vec3 l_min = rotation * glm::vec3(-scale.x, scale.y, 0.f) + position;
	glm::vec3 l_max = rotation * glm::vec3(-scale.x, -scale.y, 0.f) + position;
	glm::vec3 l_norm = rotation_matrix * glm::vec3(-1.f, 0.f, 0.f);
	// right line
	glm::vec3 r_min = rotation * glm::vec3(scale.x, scale.y, 0.f) + position;
	glm::vec3 r_max = rotation * glm::vec3(scale.x, -scale.y, 0.f) + position;
	glm::vec3 r_norm = rotation_matrix * glm::vec3(1.f, 0.f, 0.f);
	// upper line
	glm::vec3 u_min = rotation * glm::vec3(-scale.x, scale.y, 0.f) + position;
	glm::vec3 u_max = rotation * glm::vec3(scale.x, scale.y, 0.f) + position;
	glm::vec3 u_norm = rotation_matrix * glm::vec3(0.f, 1.f, 0.f);
	// bottom line
	glm::vec3 d_min = rotation * glm::vec3(-scale.x, -scale.y, 0.f) + position;
	glm::vec3 d_max = rotation * glm::vec3(scale.x, -scale.y, 0.f) + position;
	glm::vec3 d_norm = rotation_matrix * glm::vec3(0.f, -1.f, 0.f);

	std::vector<CollisionManager::line_segment> lines = {
		line_segment(l_min, l_max, l_norm),
		line_segment(r_min, r_max, r_norm),
		line_segment(u_min, u_max, u_norm),
		line_segment(d_min, d_max, d_norm)
	};
	
	return lines;
}

//performs circle-line-segment collisions on a circle and the line_segments and returns the points of intersection
std::vector<CollisionManager::intersection> CollisionManager::get_collisions(const CollisionManager::circle &c) {
	//https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm
	std::vector<CollisionManager::intersection> collision_data;

	for (CollisionManager::line_segment l : line_segments) { //iterate through each line segment
		glm::vec2 start = l.ep1;
		glm::vec2 end = l.ep2;
		glm::vec2 circle_center = c.center;
		float radius = c.radius;

		glm::vec2 d = end - start;
		glm::vec2 f = start - circle_center;

		float a = glm::dot(d, d);
		float b = 2.f * glm::dot(f, d);
		float c = glm::dot(f, f) - radius * radius;

		float discriminant = b * b - 4.f * a * c;
		if (discriminant >= 0.f) { //we hit the circle in some way
			discriminant = std::sqrt(discriminant);

			assert(a != 0.f);
			float t1 = (-b - discriminant) / (2.f * a);
			float t2 = (-b + discriminant) / (2.f * a);

			if (t1 == t2) { //segment intersects tangent to circle
				glm::vec2 point_of_intersection = start + t1 * d;
				collision_data.emplace_back(point_of_intersection, l.surface_normal);
			}
			else if (t1 >= 0.f && t1 <= 1.f && t2 >= 0.f && t2 <= 1.f) { //segment impales the circle
				glm::vec2 point_of_intersection_1 = start + t1 * d;
				glm::vec2 point_of_intersection_2 = start + t2 * d;
				glm::vec2 midpoint = (point_of_intersection_1 + point_of_intersection_2) / 2.0f;
				collision_data.emplace_back(midpoint, l.surface_normal);
			}
			else if (t1 >= 0.f && t1 <= 1.f) { //segment starts outside the circle and ends inside (poke)
				glm::vec2 point_of_intersection = start + t1 * d;
				collision_data.emplace_back(point_of_intersection, l.surface_normal);
			}
			else if (t2 >= 0.f && t2 <= 1.f) { //segment starts inside the circle and exits (exit wound)
				glm::vec2 point_of_intersection = start + t2 * d;
				collision_data.emplace_back(point_of_intersection, l.surface_normal);
			}
			/*
			else { no intersection! }
			*/
		}
	}

	return collision_data;
}

//performs capsule collision between a circle and a line segment and returns the point of intersection
CollisionManager::intersection CollisionManager::get_capsule_collision(const CollisionManager::circle &c, const CollisionManager::line_segment &l, bool &is_hit) {
	glm::vec2 point = c.center;
	float radius = c.radius;
	glm::vec2 start = l.ep1;
	glm::vec2 end = l.ep2;
	is_hit = true;

	//test the line segment first
	//project point onto line segment: https://stackoverflow.com/questions/10301001/perpendicular-on-a-line-segment-from-a-given-point
	float t = ((point.x - start.x) * (end.x - start.x) + (point.y - start.y) * (end.y - start.y)) / (powf(end.x - start.x, 2) + powf(end.y - start.y, 2));
	if (t >= 0.f && t <= 1.f) {
		glm::vec2 projection = start + t * (end - start);
		float distance = glm::distance(point, projection);

		if (distance <= radius) {
			glm::vec2 closest_exterior_point = projection + radius * l.surface_normal;
			return CollisionManager::intersection(closest_exterior_point, l.surface_normal);
		}
	}

	//test the endpoints of the line segment
	if (glm::distance(point, start) <= radius) {
		assert(point != start);
		glm::vec2 surface_normal = glm::normalize(point - start);
		glm::vec2 closest_exterior_point = start + radius * surface_normal;
		return CollisionManager::intersection(closest_exterior_point, surface_normal);
	}
	if (glm::distance(point, end) <= radius) {
		assert(point != end);
		glm::vec2 surface_normal = glm::normalize(point - end);
		glm::vec2 closest_exterior_point = end + radius * surface_normal;
		return CollisionManager::intersection(closest_exterior_point, surface_normal);
	}

	is_hit = false;
	return CollisionManager::intersection(glm::vec2(0.f), glm::vec2(0.f)); //point isn't within the capsule
}