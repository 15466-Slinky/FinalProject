#include "LitColorTextureProgram.hpp"

#include "PlayMode.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"
#include "hex_dump.hpp"

#include <glm/gtc/type_ptr.hpp>

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
			assert(drawable.transform->position.z == 0.0f);	// make sure on xy plane
		}else if(drawable_name == "CatTail"){
			cat_tail = drawable.transform;
			assert(drawable.transform->position.z == 0.0f);
		}else if(drawable_name == "Doughnut"){
			doughnut = drawable.transform;
			assert(drawable.transform->position.z == 0.0f);
		}else if(drawable_name.find("Platform") != std::string::npos){
			platforms.push_back(drawable.transform);
			assert(drawable.transform->position.z == 0.0f);
		}
	}
	
	// check all loaded
	if (platforms.empty()) throw std::runtime_error("Platforms not found.");
	assert(platforms.size() == 9);	// make sure platform count matched
	if(cat_head == nullptr) throw std::runtime_error("Cat head not found.");
	if(cat_tail == nullptr) throw std::runtime_error("Cat tail not found.");
	if(doughnut == nullptr) throw std::runtime_error("Doughnut not found.");
	
	head_pos.x = cat_head->position.x;
	head_pos.y = cat_head->position.y;
	
	tail_pos.x = cat_tail->position.x;
	tail_pos.y = cat_tail->position.y;

	for(auto p : platforms) {
		line_segments.emplace_back(get_upper_line(p));
	}

	// get pointer to camera
	if (scene.cameras.size() != 1) throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	scene.cameras.emplace_back(&scene.transforms.back());
	
	camera = &scene.cameras.front();	// used perspective camera, may not need to change fov
	camera -> fovy = glm::radians(35.0f);		// adjust fov
	//camera -> fovy = glm::radians(60.0f);
	//camera -> near = 0.01f;
	
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.repeat) {
			//ignore repeats
		} else if (evt.key.keysym.sym == SDLK_a) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_a) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_d) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_w) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_s) {
			down.pressed = false;
			return true;
		}
	}

	return false;
}

void PlayMode::update(float elapsed) {
	head_vel.x = 0;
	head_vel.y = 0;
	if(left.pressed) head_vel.x = -PLAYER_SPEED;
	else if(right.pressed) head_vel.x = PLAYER_SPEED;
	if(up.pressed) head_vel.y = PLAYER_SPEED;
	else if(down.pressed) head_vel.y = -PLAYER_SPEED;
	
	glm::vec2 dist = (head_pos - tail_pos) - playerlength;
	glm::vec2 spring_force = dist * k;
	tail_vel += spring_force * elapsed;
	tail_vel *= 0.99f; //velocity damping

	circle head_circle(head_pos, 1.f);
	auto collisions = get_collisions(head_circle, line_segments);

	for(intersection &i : collisions) {
		printf("%f %f \t %f %f\n", i.point_of_intersection.x, i.point_of_intersection.y, i.surface_normal.x, i.surface_normal.y);
	}

	// Do phyics update
	head_pos += head_vel * elapsed;
	tail_pos += tail_vel * elapsed;

	tail_vel *= .99f;

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;

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
	
	//draw scene
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

std::vector<PlayMode::intersection> PlayMode::get_collisions(PlayMode::circle c, std::vector<PlayMode::line_segment> ls) {
	//https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm
	std::vector<PlayMode::intersection> collision_data; //vector of pairs of points of intersection and normals

	for (PlayMode::line_segment l : ls) { //iterate through each line segment
		glm::vec2 start = l.ep1;
		glm::vec2 end = l.ep2;
		glm::vec2 circle_center = c.center;
		float radius = c.radius;

		glm::vec2 d = end - start;
		glm::vec2 f = start - circle_center;

		float a = glm::dot(d, d);
		float b = 2.0f * glm::dot(f, d);
		float c = glm::dot(f, f) - radius * radius;

		float discriminant = b * b - 4.0f * a * c;
		if (discriminant >= 0.0f) { //we hit the circle in some way
			discriminant = std::sqrt(discriminant);

			float t1 = (-b - discriminant) / (2.0f * a);
			float t2 = (-b + discriminant) / (2.0f * a);

			if (t1 == t2) { //segment intersects tangent to circle
				glm::vec2 point_of_intersection = start + t1 * d;
				glm::vec2 surface_normal = glm::normalize(point_of_intersection - circle_center);
				collision_data.emplace_back(point_of_intersection, surface_normal);
			}
			else if (t1 >= 0.0f && t1 <= 1.0f && t2 >= 0.0f && t2 <= 1.0f) { //segment impales the circle
				glm::vec2 point_of_intersection_1 = start + t1 * d;
				glm::vec2 point_of_intersection_2 = start + t2 * d;
				glm::vec2 midpoint = (point_of_intersection_1 + point_of_intersection_2) / 2.0f;
				glm::vec2 surface_normal = glm::normalize(midpoint - circle_center);
				collision_data.emplace_back(midpoint, surface_normal);
			}
			else if (t1 >= 0.0f && t1 <= 1.0f) { //segment starts outside the circle and ends inside (poke)
				glm::vec2 point_of_intersection = start + t1 * d;
				glm::vec2 dir = point_of_intersection - circle_center;
				glm::vec2 surface_normal = glm::normalize(glm::vec2(-dir.y, dir.x)); //may not be the right direction
				collision_data.emplace_back(point_of_intersection, surface_normal);
			}
			else if (t2 >= 0.0f && t2 <= 1.0f) { //segment starts inside the circle and exits (exit wound)
				glm::vec2 point_of_intersection = start + t2 * d;
				glm::vec2 dir = point_of_intersection - circle_center;
				glm::vec2 surface_normal = glm::normalize(glm::vec2(-dir.y, dir.x)); //may not be the right direction
				collision_data.emplace_back(point_of_intersection, surface_normal);
			}
			/*
			else { no intersection! }
			*/
		}
	}

	return collision_data;
}

PlayMode::intersection PlayMode::get_capsule_collision(PlayMode::circle c, PlayMode::line_segment l) {
	glm::vec2 point = c.center;
	float radius = c.radius;
	glm::vec2 start = l.ep1;
	glm::vec2 end = l.ep2;

	//test the line segment first
	//project point onto line segment: https://stackoverflow.com/questions/10301001/perpendicular-on-a-line-segment-from-a-given-point
	float t = ((point.x - start.x) * (end.x - start.x) + (point.y - start.y) * (end.y - start.y)) / (pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
	if (t >= 0.0f && t <= 1.0f) {
		glm::vec2 projection = start + t * (end - start);
		float distance = glm::distance(point, projection);

		if (distance <= radius) {
			glm::vec2 surface_normal = glm::normalize(point - projection);
			glm::vec2 closest_exterior_point = projection + radius * surface_normal;

			return PlayMode::intersection(closest_exterior_point, surface_normal);
		}
	}

	//test the endpoints of the line segment
	if (glm::distance(point, start) <= radius) {
		glm::vec2 surface_normal = glm::normalize(point - start);
		glm::vec2 closest_exterior_point = start + radius * surface_normal;

		return PlayMode::intersection(closest_exterior_point, surface_normal);
	}
	if (glm::distance(point, end) <= radius) {
		glm::vec2 surface_normal = glm::normalize(point - end);
		glm::vec2 closest_exterior_point = end + radius * surface_normal;

		return PlayMode::intersection(closest_exterior_point, surface_normal);
	}

	return PlayMode::intersection(glm::vec2(0.0f), glm::vec2(0.0f)); //point isn't within the capsule
}

// TODO: load all surface line segment, use local_to_world to handle rotated platform
PlayMode::line_segment PlayMode::get_upper_line(Scene::Transform* platform){
	// z component is always 0
	// +y is up, +x is right
	
	glm::vec3 position = platform->position;
	glm::vec3 scale = platform->scale;
	
	glm::vec2 min = glm::vec2(position.x - scale.x, position.y + scale.y);	//TODO: check the actual position return when test run
	glm::vec2 max = glm::vec2(position.x + scale.x, position.y + scale.y);
	
	PlayMode::line_segment line(min, max);
	
	return line;
}
