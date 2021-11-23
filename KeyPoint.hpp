#pragma once

#include <glm/glm.hpp>

#include "Scene.hpp"

#include <vector>
#include <list>

struct Grab_Point {
	glm::vec2 position;
	float past_player_dist;
	
	Grab_Point(glm::vec2 position_) : position{position_} {
		past_player_dist = 0;
	}
};

struct Spring_Point {
	glm::vec2 pos;
	glm::vec2 vel;

	Spring_Point(glm::vec2 pos_, glm::vec2 vel_): pos{pos_}, vel{vel_} {}
};

struct Check_Point {
	std::string name;
	bool reached;
	glm::vec2 position;

	Scene::Transform* box_front;
	Scene::Transform* box_left;
	Scene::Transform* box_right;

	Check_Point() = default;
	Check_Point(std::string name_, glm::vec2 position_) 
		: name{name_}, reached{false}, position{position_},
		box_front{nullptr}, box_left{nullptr}, box_right{nullptr} {}

	bool box_has_sides() {
		return (box_left != nullptr && box_right != nullptr && box_front != nullptr);
	}

	bool find_sides(std::list<Scene::Drawable> drawables) {
		std::string prefix = name.substr(0, name.find(".Checkpoint"));

		for (auto &drawable : drawables) {
			std::string drawable_name = drawable.transform->name;

			if (drawable_name == prefix + ".Front") box_front = drawable.transform;
			else if (drawable_name == prefix + ".Left") box_left = drawable.transform;
			else if (drawable_name == prefix + ".Right") box_right = drawable.transform;

			if (box_has_sides()) return true;
		}
		return false;
	}

	bool operator < (const Check_Point& c) const {
		return position.x < c.position.x;
	}
};