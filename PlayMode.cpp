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
		}else if(drawable_name == "CatTail"){
			cat_tail = drawable.transform;
		}else if(drawable_name == "Doughnut"){
			doughnut = drawable.transform;
		}else if(drawable_name.find("Platform") != std::string::npos){
			platforms.push_back(drawable.transform);
		}
	}
	
	// check all loaded
	if (platforms.empty()) throw std::runtime_error("Platforms not found.");
	assert(platforms.size() == 9);	// make sure platform count matched
	if(cat_head == nullptr) throw std::runtime_error("Cat head not found.");
	if(cat_tail == nullptr) throw std::runtime_error("Cat tail not found.");
	if(doughnut == nullptr) throw std::runtime_error("Doughnut not found.");
	
	// get pointer to camera
	if (scene.cameras.size() != 1) throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	scene.cameras.emplace_back(&scene.transforms.back());
	
	camera = &scene.cameras.front();	// used orthographic camera, may not need to change fov
	camera -> fovy = glm::radians(60.0f);
	camera -> near = 0.01f;
	
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
	//TODO: remove multiplayer stuff

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
	std::vector<std::pair<glm::vec2, glm::vec2>> collision_data; //vector of pairs of points of intersection and normals

	for (PlayMode::line_segment l : ls) { //iterate through each line segment

		glm::vec2 start = l.first;
		glm::vec2 end = l.second;
		glm::vec2 circle_center = c.first;
		float radius = c.second;

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
				glm::vec2 surface_normal = point_of_intersection - circle_center;
				collision_data.emplace_back(point_of_intersection, surface_normal);
			}
			else if (t1 >= 0.0f && t1 <= 1.0f) { //fully impaling the circle or starts outside the circle and ends inside
				std::cout << "ERROR: Circle is being impaled or poked!" << std::endl;
			}
			else if (t2 >= 0.0f && t2 <= 1.0f) { //segment starts inside the circle and exits
				std::cout << "ERROR: Circle has an exit wound!" << std::endl;
			}
			/*
			else { no intersection! }
			*/
		}
	}

	return collision_data;
}