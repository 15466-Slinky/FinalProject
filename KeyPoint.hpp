#pragma once

#include <glm/glm.hpp>

#include "Scene.hpp"
#include "Player.hpp"

#include <vector>
#include <list>

struct Grab_Point {
	glm::vec2 position;
	float past_player_dist;
	
	Grab_Point(glm::vec2 position_) : position{position_} {
		past_player_dist = 0;
	}
};

struct Check_Point {
	std::string name;
	bool reached;
	glm::vec2 position;

	bool activating;
	float time;

	Scene::Transform* box_front;
	Scene::Transform* box_left;
	Scene::Transform* box_right;

	Check_Point() = default;
	Check_Point(std::string name_, glm::vec2 position_) 
		: name{name_}, reached{false}, position{position_}, activating{false}, time{0.f},
		box_front{nullptr}, box_left{nullptr}, box_right{nullptr} {}

	bool box_has_sides();
	bool box_find_sides(std::list<Scene::Drawable> &drawables);
	void activate(float elapsed);

	bool operator < (const Check_Point& c) const {
		return position.x < c.position.x;
	}
};

void sort_checkpoints(std::vector<Check_Point> &checkpoints);
void update_checkpoints(std::vector<Check_Point> &checkpoints, long unsigned int &curr_checkpoint_id, Player &player, float elapsed);