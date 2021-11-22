#pragma once

#include "Scene.hpp"

#include <glm/glm.hpp>

struct DynamicCamera {

	DynamicCamera() = default;
	DynamicCamera(Scene::Camera* camera_, glm::vec3 camera_pos_, float camera_speed_, float default_z_);

	Scene::Camera* camera;
	glm::vec3 camera_pos;
	float camera_speed;
	float default_z;
	float zoom_out_ratio;

	void update(float elapsed, glm::vec2 target, bool extending, float zoom_out_ratio_);
	void set(glm::uvec2 const &drawable_size);

};