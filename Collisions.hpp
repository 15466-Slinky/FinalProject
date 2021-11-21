#pragma once

#include "Scene.hpp"

#include <glm/glm.hpp>

#include <vector>

struct CollisionManager {

	CollisionManager() = default;
	CollisionManager(std::vector<Scene::Transform*> platforms);

	//Collision Structs
	struct line_segment {
		glm::vec2 ep1;
		glm::vec2 ep2;
		glm::vec2 surface_normal;

		line_segment(glm::vec2 ep1_, glm::vec2 ep2_, glm::vec2 surface_normal_) 
		: ep1{ep1_}, ep2{ep2_},
		  surface_normal{glm::normalize(surface_normal_)} {
			assert(ep1 != ep2); //we do not allow degenerate line segments
			assert(surface_normal != glm::vec2(0.f)); //do not allow zero-vector to be a surface normal
		}
	};
	struct circle {
		glm::vec2 center;
		float radius;

		circle(glm::vec2 center_, float radius_) : center{center_}, radius{radius_} {
			assert(radius_ != 0.f); //we do not allow degenerate circles
		}
	};
	struct intersection {
		glm::vec2 point_of_intersection;
		glm::vec2 surface_normal;

		intersection(glm::vec2 point_of_intersection_, glm::vec2 surface_normal_)
		: point_of_intersection{point_of_intersection_},
		  surface_normal{glm::normalize(surface_normal_)} {}
	};

	//Collision Features
	std::vector<line_segment> line_segments;

	//Collision Functions
	std::vector<line_segment> get_lines(const Scene::Transform* platform);
	std::vector<line_segment> get_lines_all(const std::vector<Scene::Transform*> platforms);
	std::vector<intersection> get_collisions_all(const circle &c) const;
	intersection get_capsule_collision(const circle &c, const line_segment &l, bool &is_hit) const;
};