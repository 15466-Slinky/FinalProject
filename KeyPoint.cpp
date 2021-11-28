#include "KeyPoint.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>	//TODO: only for debug, can delete
#include <algorithm>

#include <iostream>
#include <algorithm>

bool Check_Point::box_has_sides() {
	return (box_left != nullptr && box_right != nullptr && box_front != nullptr);
}

bool Check_Point::box_find_sides(std::list<Scene::Drawable> &drawables) {
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

void Check_Point::activate(float elapsed) {
	assert(activating);
	assert(box_has_sides());

	float rot_speed = 3.14159f / 2;
	{ //make the front of the box fall down
		glm::vec3 euler_rot = glm::vec3(time * rot_speed, 0.f, 0.f);
		box_front->rotation = glm::quat(euler_rot);
	}
	{ //make the left side of the box fall down
		glm::vec3 euler_rot = glm::vec3(0.f, 0.f, time * rot_speed);
		box_left->rotation = glm::quat(euler_rot);
	}
	{ //make the right side of the box fall down
		glm::vec3 euler_rot = glm::vec3(0.f, 0.f, -time * rot_speed);
		box_right->rotation = glm::quat(euler_rot);
	}
	time += elapsed;

	if (time >= 1.f) {
		time = 0.f;
		activating = false;
	}
}

void sort_checkpoints(std::vector<Check_Point> &checkpoints) {
	std::sort(checkpoints.begin(), checkpoints.end());
}

void update_checkpoints(std::vector<Check_Point> &checkpoints, long unsigned int &curr_checkpoint_id, Player &player, float elapsed) {
	//update animation for any checkpoints which are opening
	for (size_t i=0;i<checkpoints.size();i++) {
		Check_Point &c = checkpoints[i];
		if (c.activating)
			c.activate(elapsed);
	}

	if (curr_checkpoint_id == checkpoints.size() - 1) { //we are already past the last checkpoint!
		return;
	}
	//if we are past the next checkpoint, then make it the new current checkpoint
	Check_Point &next_checkpoint = checkpoints[curr_checkpoint_id + 1];
	if (glm::distance(player.head_pos, next_checkpoint.position) <= 3.f || glm::distance(player.tail_pos, next_checkpoint.position) <= 3.f) {
		curr_checkpoint_id += 1;

		Check_Point &curr_checkpoint = checkpoints[curr_checkpoint_id];
		curr_checkpoint.reached = true;

		player.head_respawn_pos = curr_checkpoint.position;
		player.tail_respawn_pos = player.head_respawn_pos - glm::vec2(1.f, 0.f);
		curr_checkpoint.activating = true;
	}
}