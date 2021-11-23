#pragma once

#include <glm/glm.hpp>

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