#include "DynamicCamera.hpp"

DynamicCamera::DynamicCamera(Scene::Camera* camera_, glm::vec3 camera_pos_, float camera_speed_, float default_z_) 
: camera{camera_}, camera_speed{camera_speed_}, default_z{default_z_}
{
	camera->fovy = glm::radians(35.f); // adjust fov
	//camera -> fovy = glm::radians(60.0f);
	//camera -> near = 0.01f;
	camera_pos = glm::vec3(camera_pos_.x, camera_pos_.y, default_z);
	camera->transform->position = camera_pos;
	zoom_out_ratio = 1.f;
}

void DynamicCamera::update(float elapsed, glm::vec2 target, bool extending, float zoom_out_ratio_) {
	glm::vec2 camera_pos_2d = glm::vec2(camera_pos);
	glm::vec2 dir = target - camera_pos_2d;

	camera_pos.x += dir.x * elapsed * camera_speed;
	camera_pos.y += dir.y * elapsed * camera_speed;
	
	if (extending) { //zoom out if player is stretched far enough
		zoom_out_ratio = std::max(1.f, zoom_out_ratio_);
		camera_pos.z = default_z * zoom_out_ratio;
	}
	else if (camera_pos.z > default_z){ //zoom back in over time
		zoom_out_ratio = std::max(1.f, zoom_out_ratio - elapsed * camera_speed / 5.f);
		camera_pos.z = default_z * zoom_out_ratio;
	}
}

void DynamicCamera::draw(glm::uvec2 const &drawable_size) {
	//draw scene and update camera
	camera->transform->position = camera_pos;
	camera->aspect = float(drawable_size.x) / float(drawable_size.y);
}