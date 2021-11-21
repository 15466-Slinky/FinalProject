#pragma once

#include <glm/glm.hpp>

struct Player {

	Player() = default;

	float speed = 10.f;
	float jump_speed = 10.f;
	float grab_radius = 1.5f;
};