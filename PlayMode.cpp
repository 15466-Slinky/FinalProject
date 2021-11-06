#include "LitColorTextureProgram.hpp"

#include "PlayMode.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"
#include "hex_dump.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <random>
#include <cmath>

GLuint slinky_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > slinky_meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("slinky.pnct"));
	slinky_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > slinky_scene(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("slinky.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = slinky_meshes->lookup(mesh_name);

		//get 4 pairs of shapes
		scene.drawables.emplace_back(transform);

		Scene::Drawable &drawable = scene.drawables.back();

		drawable.pipeline = lit_color_texture_program_pipeline;

		drawable.pipeline.vao = slinky_meshes_for_lit_color_texture_program;
		drawable.pipeline.type = mesh.type;
		drawable.pipeline.start = mesh.start;
		drawable.pipeline.count = mesh.count;
	});
});


PlayMode::PlayMode() : scene(*slinky_scene) {
	// get pointer to each shape
	for (auto &drawable : scene.drawables) {
		std::string drawable_name = drawable.transform->name;
		
		if(drawable_name == "CatHead"){
			cat_head = drawable.transform;
			assert(drawable.transform->position.z == 0.f);	// make sure on xy plane
		}else if(drawable_name == "CatTail"){
			cat_tail = drawable.transform;
			assert(drawable.transform->position.z == 0.f);
		}else if(drawable_name == "Doughnut"){
			doughnut = drawable.transform;
			assert(drawable.transform->position.z == 0.f);
		}else if(drawable_name.find("Platform") != std::string::npos){
			platforms.push_back(drawable.transform);
			assert(drawable.transform->position.z == 0.f);
		}else if(drawable_name.find("Checkpoint") != std::string::npos){
			checkpoints.emplace_back(glm::vec2(drawable.transform->position.x, drawable.transform->position.y));
			//checkpoints don't have to be at z-value 0.f for visual reasons
		}
	}
	
	// check all loaded
	if (platforms.empty()) throw std::runtime_error("Platforms not found.");
	assert(platforms.size() == 10); // make sure platform count matched
	if (checkpoints.empty()) throw std::runtime_error("Checkpoints not found.");
	assert(checkpoints.size() == 1); //make sure the checkpoint count matches
	if(cat_head == nullptr) throw std::runtime_error("Cat head not found.");
	if(cat_tail == nullptr) throw std::runtime_error("Cat tail not found.");
	if(doughnut == nullptr) throw std::runtime_error("Doughnut not found.");

	curr_checkpoint_id = -1; //we haven't reached any checkpoint yet
	next_checkpoint = checkpoints[0];
	
	head_pos.x = cat_head->position.x;
	head_pos.y = cat_head->position.y;

	head_start = head_pos;
	tail_start = tail_pos;
	
	tail_pos.x = cat_tail->position.x;
	tail_pos.y = cat_tail->position.y;

	head_vel = glm::vec2(0.f, 0.f);
	tail_vel = glm::vec2(0.f, 0.f);

	head_grounded = false;
	tail_grounded = false;

	for(auto p : platforms) {
		std::vector<PlayMode::line_segment> lines = get_lines(p);
		assert(lines.size() == 4);
		
		line_segments.emplace_back(lines[0]);	// left
		line_segments.emplace_back(lines[1]);	// right
		line_segments.emplace_back(lines[2]);	// up
		line_segments.emplace_back(lines[3]);	// bottom
	}

	// get pointer to camera
	if (scene.cameras.size() != 1) throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	scene.cameras.emplace_back(&scene.transforms.back());
	
	camera = &scene.cameras.front();	// used perspective camera, may not need to change fov
	camera -> fovy = glm::radians(35.f);		// adjust fov
	//camera -> fovy = glm::radians(60.0f);
	//camera -> near = 0.01f;
	camera_pos = camera->transform->position;
	camera_default_z = camera_pos.z;
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.repeat) {
			//ignore repeats
		} else if (evt.key.keysym.sym == SDLK_a || evt.key.keysym.sym == SDLK_LEFT) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d || evt.key.keysym.sym == SDLK_RIGHT) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w || evt.key.keysym.sym == SDLK_UP) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s || evt.key.keysym.sym == SDLK_DOWN) {
			down.downs += 1;
			down.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_SPACE) {
			space.downs += 1;
			space.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_q) {
			if (grab_ledge(tail_pos, 1.f + grab_radius)) {
				fixed_tail = !fixed_tail;
			}
			return true;
		} else if (evt.key.keysym.sym == SDLK_e) {
			if (grab_ledge(head_pos, 1.f + grab_radius)) {
				fixed_head = !fixed_head;
			}
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_a || evt.key.keysym.sym == SDLK_LEFT) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d || evt.key.keysym.sym == SDLK_RIGHT) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w || evt.key.keysym.sym == SDLK_UP) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s || evt.key.keysym.sym == SDLK_DOWN) {
			down.pressed = false;
			return true;
		}else if (evt.key.keysym.sym == SDLK_SPACE) {
			space.pressed = false;
			return true;
		}
	}
	return false;
}

void PlayMode::update(float elapsed) {
	//respawn if player fell to their death
	if (head_pos.y < DEATH_BOUND && tail_pos.y < DEATH_BOUND)
		respawn();

	// Apply gravity
	head_vel.y -= elapsed * GRAVITY;
	tail_vel.y -= elapsed * GRAVITY;
	
	playerlength = space.pressed ? 20.f : 1.f;
	if (space.pressed) {
		stretched = true;
	}
	else if (stretched && glm::distance(head_pos, tail_pos) <= 4.f) {
		stretched = false;

		fixed_head = false;
		head_grounded = false;
		head_vel += tail_vel * 0.5f;
		tail_vel *= 0.5f;

		printf("Recompressed %f\n", head_vel.x);
	}


	if (!fixed_head && !fixed_tail) {
		free_movement(elapsed);
	} else if (fixed_head && fixed_tail) {
		head_vel.x = 0.f;
		head_vel.y = 0.f;
		tail_vel.x = 0.f;
		tail_vel.y = 0.f;
		std::cout << "you have stuck both your head and tail and cannot move\n";
	} else if (fixed_head) {
		fixed_head_movement(elapsed);
	} else if (fixed_tail) {
		fixed_tail_movement(elapsed);
	}

	// Do phyics update
	head_pos += head_vel * elapsed;
	tail_pos += tail_vel * elapsed;

	//update camera
	update_camera(elapsed);

	//update checkpoint
	update_checkpoint();

	// Check collision with the walls and adjust the velocities accordingly
	collide_segments(head_pos, head_vel, 1.f, head_grounded);
	collide_segments(tail_pos, tail_vel, 1.f, tail_grounded);

	// Air resistance, or surface friction if the segment is sitting on the floor
	//head_vel *= head_grounded ? .9f : .99f;
	//tail_vel *= tail_grounded ? .9f : .99f; 
	// Air resistance only
	head_vel *= 0.99f;
	tail_vel *= 0.99f; 

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
	space.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
//	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT);

//	{ //use DrawLines to overlay some text:
//		glDisable(GL_DEPTH_TEST);
//		float aspect = float(drawable_size.x) / float(drawable_size.y);
//		DrawLines lines(glm::mat4(
//			1.0f / aspect, 0.0f, 0.0f, 0.0f,
//			0.0f, 1.0f, 0.0f, 0.0f,
//			0.0f, 0.0f, 1.0f, 0.0f,
//			0.0f, 0.0f, 0.0f, 1.0f
//		));
//
//		auto draw_text = [&](glm::vec2 const &at, std::string const &text, float H) {
//			lines.draw_text(text,
//				glm::vec3(at.x, at.y, 0.0),
//				glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
//				glm::u8vec4(0x00, 0x00, 0x00, 0x00));
//			float ofs = 2.0f / drawable_size.y;
//			lines.draw_text(text,
//				glm::vec3(at.x + ofs, at.y + ofs, 0.0),
//				glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
//				glm::u8vec4(0xff, 0xff, 0xff, 0x00));
//		};
//
//		draw_text(glm::vec2(-aspect + 0.1f, 0.0f), server_message, 0.09f);
//
//		draw_text(glm::vec2(-aspect + 0.1f,-0.9f), "(press WASD to change your total)", 0.09f);
//	}

	cat_head->position.x = head_pos.x;
	cat_head->position.y = head_pos.y;

	cat_tail->position.x = tail_pos.x;
	cat_tail->position.y = tail_pos.y;
	
	//draw scene and update camera
	camera->transform->position = camera_pos;
	camera->aspect = float(drawable_size.x) / float(drawable_size.y);

	//set up light type and position for lit_color_texture_program:
	// TODO: consider using the Light(s) in the scene to do this
	glUseProgram(lit_color_texture_program->program);
	glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
	glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	glUseProgram(0);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	scene.draw(*camera);
	
	GL_ERRORS();
}

std::vector<PlayMode::intersection> PlayMode::get_collisions(const PlayMode::circle &c, const std::vector<PlayMode::line_segment> &ls) {
	//https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm
	std::vector<PlayMode::intersection> collision_data;

	for (PlayMode::line_segment l : ls) { //iterate through each line segment
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

PlayMode::intersection PlayMode::get_capsule_collision(const PlayMode::circle &c, const PlayMode::line_segment &l, bool &is_hit) {
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
			return PlayMode::intersection(closest_exterior_point, l.surface_normal);
		}
	}

	//test the endpoints of the line segment
	if (glm::distance(point, start) <= radius) {
		assert(point != start);
		glm::vec2 surface_normal = glm::normalize(point - start);
		glm::vec2 closest_exterior_point = start + radius * surface_normal;
		return PlayMode::intersection(closest_exterior_point, surface_normal);
	}
	if (glm::distance(point, end) <= radius) {
		assert(point != end);
		glm::vec2 surface_normal = glm::normalize(point - end);
		glm::vec2 closest_exterior_point = end + radius * surface_normal;
		return PlayMode::intersection(closest_exterior_point, surface_normal);
	}

	is_hit = false;
	return PlayMode::intersection(glm::vec2(0.f), glm::vec2(0.f)); //point isn't within the capsule
}

// TODO: load all surface line segment, use local_to_world to handle rotated platform
std::vector<PlayMode::line_segment> PlayMode::get_lines(const Scene::Transform* platform){
	// z component is always 0
	// +y is up, +x is right
	
	glm::vec3 position = platform->position;
	glm::quat rotation = platform->rotation;
	glm::mat3 rotation_matrix = glm::mat3_cast(rotation);
	glm::vec3 scale = platform->scale;
	//glm::mat4x3 to_world = platform->make_local_to_world();
	
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

	/*
	// left line
	glm::vec3 l_min = to_world * glm::vec4(-scale.x, scale.y, 0.0f, 1.0f);
	glm::vec3 l_max = to_world * glm::vec4(-scale.x, -scale.y, 0.0f, 1.0f);
	// right line
	glm::vec3 r_min = to_world * glm::vec4(scale.x, scale.y, 0.0f, 1.0f);
	glm::vec3 r_max = to_world * glm::vec4(scale.x, -scale.y, 0.0f, 1.0f);
	// upper line
	glm::vec3 u_min = to_world * glm::vec4(-scale.x, scale.y, 0.0f, 1.0f);
	glm::vec3 u_max = to_world * glm::vec4(scale.x, scale.y, 0.0f, 1.0f);
	// bottom line
	glm::vec3 d_min = to_world * glm::vec4(-scale.x, -scale.y, 0.0f, 1.0f);
	glm::vec3 d_max = to_world * glm::vec4(scale.x, -scale.y, 0.0f, 1.0f);
	*/

	std::vector<PlayMode::line_segment> lines = {
		line_segment(l_min, l_max, l_norm),
		line_segment(r_min, r_max, r_norm),
		line_segment(u_min, u_max, u_norm),
		line_segment(d_min, d_max, d_norm)
	};
	
	return lines;
}

void PlayMode::update_camera(float elapsed) { //numbers in this function can be changed later for fine-tuning
	glm::vec2 camera_pos_2d = glm::vec2(camera_pos.x, camera_pos.y);
	glm::vec2 camera_dir_2d = head_pos - camera_pos_2d;

	camera_pos.x += camera_dir_2d.x * elapsed * CAMERA_SPEED;
	camera_pos.y += camera_dir_2d.y * elapsed * CAMERA_SPEED;
	
	if (space.pressed) { //zoom out if player is stretched far enough
		float dist = std::max(0.f, glm::distance(head_pos, tail_pos) - playerlength);
		float zoom_out_ratio = std::max(1.f, dist / playerlength);
		camera_pos.z = camera_default_z * zoom_out_ratio;
	}
	else if (camera_pos.z > camera_default_z){ //zoom back in over time
		camera_pos.z = std::max(camera_default_z, camera_pos.z - elapsed * CAMERA_SPEED);
	}
}

void PlayMode::sort_checkpoints() {
	std::sort(checkpoints.begin(), checkpoints.end());
}

void PlayMode::update_checkpoint() {
	if (curr_checkpoint_id == checkpoints.size() - 1) { //we already passed the last checkpoint! is it game over?
		return; //we can revisit this later if we want the last checkpoint to end the game
	}
	if (head_pos.x >= next_checkpoint.position.x || tail_pos.x >= next_checkpoint.position.x) { //if we are past the next checkpoint, then make it the new current checkpoint
		curr_checkpoint_id += 1;
		curr_checkpoint = checkpoints[curr_checkpoint_id];
		curr_checkpoint.reached = true;

		if (curr_checkpoint_id != checkpoints.size() - 1)
			next_checkpoint = checkpoints[curr_checkpoint_id + 1];

		head_start = curr_checkpoint.position;
		tail_start = head_start - glm::vec2(1.f, 0.f);
	}
}

void PlayMode::collide_segments(glm::vec2 &pos, glm::vec2 &vel, float radius, bool &grounded) {
	grounded = false;

	for(line_segment &ls : line_segments) {
		circle c(pos, radius);

		bool is_hit = false;
		intersection hit = get_capsule_collision(c, ls, is_hit);

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

bool PlayMode::grab_ledge(glm::vec2& pos, float radius) {
	circle c(pos, radius);
	std::vector<intersection> hits = get_collisions(c, line_segments);
	return (!hits.empty());
}

void PlayMode::free_movement(float elapsed) {
	// If the head is grounded, just stay still
	if(head_grounded) {
		head_vel.x = 0.f;
		head_vel.y = 0.f;
	}

	if (left.pressed) head_vel.x = -PLAYER_SPEED;
	else if (right.pressed) head_vel.x = PLAYER_SPEED;
	if (up.pressed && head_grounded) 
	{	
		head_vel.y = JUMP_SPEED;
		tail_vel.y += JUMP_SPEED / 2.f;
	}

	glm::vec2 disp = (head_pos - tail_pos);
	float dist = std::max(0.f, glm::distance(head_pos, tail_pos) - playerlength);
	if (disp != glm::vec2(0.f)) {
		glm::vec2 spring_force = glm::normalize(disp) * dist * k;
		tail_vel += spring_force * elapsed;
	}
	
	// Only pull on the head if it's not standing somewhere
	//if(!head_grounded)
	//	head_vel -= spring_force * elapsed;
}

void PlayMode::fixed_head_movement(float elapsed) {
	head_vel.x = 0.f;
	head_vel.y = 0.f;

	// If the tail is grounded, just stay still
	if(tail_grounded) {
		tail_vel.x = 0.f;
		tail_vel.y = 0.f;
	}
	//tail_vel.x = 0;
	//tail_vel.y = 0;
	if (left.pressed) tail_vel.x = -PLAYER_SPEED;
	else if (right.pressed) tail_vel.x = PLAYER_SPEED;
	if (up.pressed && tail_grounded) tail_vel.y = JUMP_SPEED;

	glm::vec2 disp = (head_pos - tail_pos);
	float dist = std::max(0.f, glm::distance(head_pos, tail_pos) - playerlength);
	if (disp != glm::vec2(0.f)) {
		glm::vec2 spring_force = glm::normalize(disp) * dist * k;
		tail_vel += spring_force * elapsed;
	}
}

void PlayMode::fixed_tail_movement(float elapsed) {
	//head_vel.x = 0;
	//head_vel.y = 0;
	tail_vel.x = 0.f;
	tail_vel.y = 0.f;
	if (left.pressed) head_vel.x = -PLAYER_SPEED;
	else if (right.pressed) head_vel.x = PLAYER_SPEED;
	if (up.pressed && head_grounded) head_vel.y = JUMP_SPEED;

	glm::vec2 disp = (head_pos - tail_pos);
	float dist = std::max(0.f, glm::distance(head_pos, tail_pos) - playerlength);
	if (disp != glm::vec2(0.f)) {
		glm::vec2 spring_force = glm::normalize(disp) * dist * k;
		head_vel -= spring_force * elapsed;
	}
}

void PlayMode::respawn() {
	head_pos = head_start;
	head_vel = glm::vec2(0.f, 0.f);
	tail_pos = head_start;
	tail_pos.x -= 1.f; //to give space between head and tail
	tail_vel = glm::vec2(0.f, 0.f);
}
