#include "LitColorTextureProgram.hpp"
#include "ColorTextureProgram.hpp"
#include "load_save_png.hpp"

#include "PlayMode.hpp"
#include "MenuMode.hpp"
#include "WinMode.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"
#include "hex_dump.hpp"

#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/string_cast.hpp>	//TODO: only for debug, can delete
#include <glm/gtc/quaternion.hpp>

#include <random>
#include <cmath>
#include <math.h>
#include <time.h>

//------------ Scene and mesh loading ---------- //
GLuint slinky_meshes_for_lit_color_texture_program = 0;

// Really hacky scene loading solution where we load all of them
// I'm sorry, this is just what I had to do
Load< MeshBuffer > slinky_meshes1(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("slinky1.pnct"));
	slinky_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > slinky_scene1(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("slinky1.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = slinky_meshes1->lookup(mesh_name);

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

Load< MeshBuffer > slinky_meshes2(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("slinky2.pnct"));
	slinky_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > slinky_scene2(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("slinky2.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = slinky_meshes2->lookup(mesh_name);

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


Load< MeshBuffer > slinky_meshes3(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("slinky3.pnct"));
	slinky_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > slinky_scene3(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("slinky3.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = slinky_meshes3->lookup(mesh_name);

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


Load< MeshBuffer > slinky_meshes4(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("slinky4.pnct"));
	slinky_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > slinky_scene4(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("slinky4.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = slinky_meshes4->lookup(mesh_name);

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



Load< MeshBuffer > slinky_meshes5(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("slinky5.pnct"));
	slinky_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > slinky_scene5(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("slinky5.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = slinky_meshes5->lookup(mesh_name);

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



Load< MeshBuffer > slinky_meshes6(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("slinky6.pnct"));
	slinky_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > slinky_scene6(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("slinky6.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = slinky_meshes6->lookup(mesh_name);

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


// Array of the loaded scenes to dereference on PlayMode creation

Load< Scene >* scenes[LEVEL_CNT] = {&slinky_scene1,
									&slinky_scene2,
									&slinky_scene3,
									&slinky_scene4,
									&slinky_scene5,
									&slinky_scene6};

/*
Load< Scene >* scenes[LEVEL_CNT] = {&slinky_scene1,
									&slinky_scene2,
									&slinky_scene3,
									&slinky_scene4};
*/

//--------------- Music and SFX loading --------------//
Load< Sound::Sample > bgm_loop_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("Foam_Rubber.opus"));
});

Load< Sound::Sample > spring_boing_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("140867__juskiddink__boing.opus"));
});

Load< Sound::Sample > cat_meow_sample(LoadTagDefault, []() -> Sound::Sample const* {
	return new Sound::Sample(data_path("110011__tuberatanka__cat-meow.opus"));
});

Load< Sound::Sample > cat_scream_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("415209__inspectorj__cat-screaming-a.opus"));
});

Load< Sound::Sample > nt_effect_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("gundam-newtype-flash-sound-effect.opus"));
});

//------------- Functions -----------------//
PlayMode::PlayMode(int level) {
	srand((unsigned int)time(NULL));

	level_id = level;
	skip_level = false;

	// Pick the scene we want to load
	scene = *(*scenes[level]);

	switch (level_id) {
		case 0:
			current_bg = bg1_path;
			break;
		case 1:
			current_bg = bg2_path;
			break;
		case 2:
			current_bg = bg3_path;
			break;
		case 3:
			current_bg = bg4_path;
			break;
		case 4:
			current_bg = bg5_path;
			break;
		case 5:
			current_bg = bg6_path;
			break;
		default:
			current_bg = bg1_path;
			break;
	}

	// get pointer to each shape
	for (auto &drawable : scene.drawables) {
		std::string drawable_name = drawable.transform->name;

		if(drawable_name == "CatHead"){
			cat_head = drawable.transform;
			assert(drawable.transform->position.z == 0.f);	// make sure on xy plane
		}
		else if(drawable_name == "CatTail"){
			cat_tail = drawable.transform;
			assert(drawable.transform->position.z == 0.f);
		}
		else if(drawable_name == "CatBody"){
			cat_body = &drawable;
			assert(drawable.transform->position.z == 0.f);
		}
		else if(drawable_name == "Doughnut"){
			doughnut = drawable.transform;
			assert(drawable.transform->position.z == 0.f);
		}
		else if (drawable.transform->name == "LeftBackFoot") {
			lbpeet = drawable.transform;
			lbpeet->position = (cat_tail->make_world_to_local()) * glm::vec4(lbpeet->position.x, lbpeet->position.y, lbpeet->position.z, 1.0f);
			lbpeet->rotation = glm::inverse(cat_tail ->rotation) * lbpeet->rotation;
			lbpeet->parent = cat_tail;
		}
		else if (drawable.transform->name == "LeftFrontFoot") {
			lfpeet = drawable.transform;
			lfpeet->position = (cat_head->make_world_to_local()) * glm::vec4(lfpeet->position.x, lfpeet->position.y, lfpeet->position.z, 1.0f);
			lfpeet->rotation = glm::inverse(cat_head->rotation) * lfpeet->rotation;
			lfpeet->parent = cat_head;
		}
		else if (drawable.transform->name == "RightBackFoot") {
			rbpeet = drawable.transform;
			rbpeet->position = (cat_tail->make_world_to_local()) * glm::vec4(rbpeet->position.x, rbpeet->position.y, rbpeet->position.z, 1.0f);
			rbpeet->rotation = glm::inverse(cat_tail->rotation) * rbpeet->rotation;
			rbpeet->parent = cat_tail;
		}
		else if (drawable.transform->name == "RightFrontFoot") {
			rfpeet = drawable.transform;
			rfpeet->position = (cat_head->make_world_to_local()) * glm::vec4(rfpeet->position.x, rfpeet->position.y, rfpeet->position.z, 1.0f);
			rfpeet->rotation = glm::inverse(cat_head->rotation) * rfpeet->rotation;
			rfpeet->parent = cat_head;
		}
		else if(drawable_name.find("Platform") != std::string::npos &&
				// Don't actually collide with scratching posts
				drawable_name.find("Scratch") == std::string::npos){
			platforms.push_back(drawable.transform);
			assert(drawable.transform->position.z == 0.f);

			GLuint tex = load_texture(data_path("floortex.png"));
			drawable.pipeline.textures[0].texture = tex;
		}
		else if(drawable_name.find("Checkpoint") != std::string::npos){
			checkpoints.emplace_back(drawable_name, glm::vec2(drawable.transform->position.x, drawable.transform->position.y));
			//checkpoints don't have to be at z-value 0.f for visual reasons
			assert(checkpoints.back().box_find_sides(scene.drawables));
		}
		else if(drawable_name.find("Scratch") != std::string::npos && 
				drawable_name.find("Post") != std::string::npos) {
			grab_points.emplace_back(glm::vec2(drawable.transform->position.x, drawable.transform->position.y));
		}
		else if(drawable_name.find("Fish") != std::string::npos){
			fishes.push_back(drawable.transform);
			//fish don't have to be at z-value 0.f for visual reasons
		}
		else if (drawable_name == "Background") {
			GLuint tex = load_texture(data_path(current_bg));
			drawable.pipeline.textures[0].texture = tex;
		}
	}
	
	// check all loaded
	if (platforms.empty()) throw std::runtime_error("Platforms not found.");
	//assert(platforms.size() == 9); // make sure platform count matched
	if (checkpoints.empty()) throw std::runtime_error("Checkpoints not found.");
	//assert(checkpoints.size() == 1); //make sure the checkpoint count matches
	if(cat_head == nullptr) throw std::runtime_error("Cat head not found.");
	if(cat_tail == nullptr) throw std::runtime_error("Cat tail not found.");
	if (cat_body == nullptr) throw std::runtime_error("Cat body not found.");
	if(doughnut == nullptr) throw std::runtime_error("Doughnut not found.");
	if (lbpeet == nullptr) throw std::runtime_error("Cat left back foot not found.");
	if (rbpeet == nullptr) throw std::runtime_error("Cat right back foot not found.");
	if (lfpeet == nullptr) throw std::runtime_error("Cat left front foot not found.");
	if (rfpeet == nullptr) throw std::runtime_error("Cat right front foot not found.");

	lfpeet_base = lfpeet->position;
	lbpeet_base = lbpeet->position;
	rfpeet_base = rfpeet->position;
	rbpeet_base = rbpeet->position;

	player = Player(glm::vec2(cat_head->position), glm::vec2(cat_tail->position), glm::vec2(0.f), glm::vec2(0.f));

	sort_checkpoints(checkpoints);
	curr_checkpoint_id = -1; //we haven't reached any checkpoint yet

	collision_manager = CollisionManager(platforms);

	//check cameras and make the dynamic camera
	if (scene.cameras.size() != 1) throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
		scene.cameras.emplace_back(&scene.transforms.back());
	camera = &scene.cameras.front();
	dynamic_camera = DynamicCamera(camera, glm::vec3(0.f), 9.f, 50.f);

	//start music loop playing:
	bgm_loop = Sound::loop(*bgm_loop_sample, 1.0f, 0.0f);

	//----- allocate OpenGL resources ----- from Game 0
	{ //vertex buffer:
		glGenBuffers(1, &vertex_buffer);
		//for now, buffer will be un-filled.

		GL_ERRORS(); //PARANOIA: print out any OpenGL errors that may have happened
	}
	{ //vertex array mapping buffer for color_texture_program:
		//ask OpenGL to fill vertex_buffer_for_color_texture_program with the name of an unused vertex array object:
		glGenVertexArrays(1, &vertex_buffer_for_color_texture_program);

		//set vertex_buffer_for_color_texture_program as the current vertex array object:
		glBindVertexArray(vertex_buffer_for_color_texture_program);

		//set vertex_buffer as the source of glVertexAttribPointer() commands:
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

		//set up the vertex array object to describe arrays of PongMode::Vertex:
		glVertexAttribPointer(
			color_texture_program.Position_vec4, //attribute
			3, //size
			GL_FLOAT, //type
			GL_FALSE, //normalized
			sizeof(Vertex), //stride
			(GLbyte *)0 + 0 //offset
		);
		glEnableVertexAttribArray(color_texture_program.Position_vec4);
		//[Note that it is okay to bind a vec3 input to a vec4 attribute -- the w component will be filled with 1.0 automatically]

		glVertexAttribPointer(
			color_texture_program.Color_vec4, //attribute
			4, //size
			GL_UNSIGNED_BYTE, //type
			GL_TRUE, //normalized
			sizeof(Vertex), //stride
			(GLbyte *)0 + 4*3 //offset
		);
		glEnableVertexAttribArray(color_texture_program.Color_vec4);

		glVertexAttribPointer(
			color_texture_program.TexCoord_vec2, //attribute
			2, //size
			GL_FLOAT, //type
			GL_FALSE, //normalized
			sizeof(Vertex), //stride
			(GLbyte *)0 + 4*3 + 4*1 //offset
		);
		glEnableVertexAttribArray(color_texture_program.TexCoord_vec2);

		//done referring to vertex_buffer, so unbind it:
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//done setting up vertex array object, so unbind it:
		glBindVertexArray(0);

		GL_ERRORS(); //PARANOIA: print out any OpenGL errors that may have happened
	}
	{ //solid white texture:
		//ask OpenGL to fill white_tex with the name of an unused texture object:
		glGenTextures(1, &white_tex);

		//bind that texture object as a GL_TEXTURE_2D-type texture:
		glBindTexture(GL_TEXTURE_2D, white_tex);

		//upload a 1x1 image of solid white to the texture:
		glm::uvec2 size = glm::uvec2(1,1);
		std::vector< glm::u8vec4 > data(size.x*size.y, glm::u8vec4(0xff, 0xff, 0xff, 0xff));
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

		//set filtering and wrapping parameters:
		//(it's a bit silly to mipmap a 1x1 texture, but I'm doing it because you may want to use this code to load different sizes of texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//since texture uses a mipmap and we haven't uploaded one, instruct opengl to make one for us:
		glGenerateMipmap(GL_TEXTURE_2D);

		//Okay, texture uploaded, can unbind it:
		glBindTexture(GL_TEXTURE_2D, 0);

		GL_ERRORS(); //PARANOIA: print out any OpenGL errors that may have happened
	}
}

PlayMode::~PlayMode() {
	//----- free OpenGL resources ----- from Game 0
	glDeleteBuffers(1, &vertex_buffer);
	vertex_buffer = 0;

	glDeleteVertexArrays(1, &vertex_buffer_for_color_texture_program);
	vertex_buffer_for_color_texture_program = 0;

	glDeleteTextures(1, &white_tex);
	white_tex = 0;
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
		} else if (evt.key.keysym.sym == SDLK_e) {
			//if (player.grab_ledge(collision_manager, player.head_pos, 1.f + grab_radius)) {
			//	player.grabbing = !player.grabbing;
			//}

			player.grabbing = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_ESCAPE) {
				//TODO: need to move everything that holds playmode inplace
				Sound::stop_all_samples();
				Mode::set_current(std::make_shared< MenuMode >());
				return true;
		} else if (evt.key.keysym.sym == SDLK_F1) {
				//cheat key for testing
				skip_level = true;
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

			// play spring sfx when release
			spring_boing_SFX = Sound::play(*spring_boing_sample, 0.6f, 0.0f);

			return true;
		}
	}
	return false;
}

void PlayMode::update(float elapsed) {
	//check if the game has ended, aka if we have eaten the donut
	if (game_over)
    	celebrate_update(elapsed);

	if (glm::distance(player.head_pos, glm::vec2(doughnut->position)) < 5.f &&
		fishes.size() == 0) {
		game_over = true;
	}

	//respawn if player fell to their death
	if (player.head_pos.y < DEATH_BOUND && player.tail_pos.y < DEATH_BOUND){
		player.respawn();

		//play cat scream
		cat_scream_SFX = Sound::play(*cat_scream_sample, 1.0f, 0.0f);
	}

	//interact with objects
	interact_objects(elapsed);

	//update checkpoint
	update_checkpoints(checkpoints, curr_checkpoint_id, player, elapsed);

	player_phys_update(elapsed);
	animation_update(elapsed);

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
	space.downs = 0;

	// If we won and it's not the last level, wait 2 seconds and then enter the next level
	if((game_over && level_id + 1 < LEVEL_CNT) || skip_level) {
		level_switch_timer += elapsed;

		if(level_switch_timer > 2.f) {
			//TODO: need to move everything that holds playmode inplace
			Sound::stop_all_samples();
			Mode::set_current(std::make_shared< PlayMode >(level_id + 1));
		}
	}
	if ((game_over && level_id + 1 == LEVEL_CNT) || skip_level) {
		level_switch_timer += elapsed;
		if (level_switch_timer > 10.f) {
			Sound::stop_all_samples();
			Mode::set_current(std::make_shared< WinMode >());
			return;
		}

	}
}

/* Checks to see if the player has approached any grab points, and automatically grabs 
   onto that point if they have */
void PlayMode::do_auto_grab() {
	for(Grab_Point &p : grab_points) {
		float dist = glm::distance(p.position, player.head_pos);

		// Only perform a grab upon entry into the grab radius
		if(dist <= player.grab_radius && p.past_player_dist > player.grab_radius) {
			player.grabbing = true;
		}

		p.past_player_dist = dist;
	}
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {	
	dynamic_camera.set(drawable_size);

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

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	scene.draw(*camera);
	
	GL_ERRORS();

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	float aspect = float(drawable_size.x) / float(drawable_size.y);
	DrawLines lines(glm::mat4(
		1.0f / aspect, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	));

	auto draw_text = [&](glm::vec2 const &at, std::string const &text, float H) {
		lines.draw_text(text,
			glm::vec3(at.x, at.y, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0x00, 0x00, 0x00, 0xff));
		float ofs = 2.0f / drawable_size.y;
		lines.draw_text(text,
			glm::vec3(at.x + ofs, at.y + ofs, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0xff));
	};

	std::string fish_remaining_text = "Fish Remaining: " + std::to_string(fishes.size());
	draw_text(glm::vec2(-aspect + 0.1f, 0.8f), fish_remaining_text, 0.1f);

	if (glm::distance(player.head_pos, glm::vec2(doughnut->position)) < 5.f &&
		fishes.size() > 0) {

		draw_text(glm::vec2(-aspect + 0.5f, 0.0f), "Save the donut for dessert!", 0.1f);
	}
	if (game_over) { 
		//use DrawLines to overlay some text for the last level
		//if(level_id + 1 == LEVEL_CNT)
			//draw_text(glm::vec2(-aspect + 0.5f, 0.0f), "GAME OVER: YOU WIN!", 0.4f);

		celebrate_draw(drawable_size);
	}
}

void PlayMode::spin_fish(float elapsed) {
	float spin_speed = 1.f;

	for (size_t i=0;i<fishes.size();i++) {
		//spin along the x-axis
		glm::vec3 euler_rot = glm::vec3(elapsed * spin_speed, 0.f, 0.f);
		fishes[i]->rotation *= glm::quat(euler_rot);
	}
}

void PlayMode::celebrate_update(float elapsed) {
	fireworks_countdown -= elapsed;

	//release a new firework if we can
	if (fireworks_countdown <= 0.f && fireworks.size() < max_fireworks_num) {
		fireworks_countdown = fireworks_time_between;
		fireworks.emplace_back(fireworks_speed, glm::vec2(0.f));
	}

	//update each firework
	for (size_t i=0;i<fireworks.size();i++) {
		PlayMode::firework &f = fireworks[i];

		f.update(elapsed);
		if (f.age >= max_fireworks_age) {
			fireworks.erase(fireworks.begin()+i);
			i--;
		}
	}
}

void PlayMode::celebrate_draw(glm::uvec2 const &drawable_size) {
	std::vector<PlayMode::Vertex> vertices;

	for (PlayMode::firework f : fireworks) {
		//https://stackoverflow.com/questions/9879258/how-can-i-generate-random-points-on-a-circles-circumference-in-javascript
		for (size_t i=0;i<3;i++) {
			float x = cosf((rand() / (float)RAND_MAX) * (float)M_PI * 2.f) + f.position.x;
			float y = sinf((rand() / (float)RAND_MAX) * (float)M_PI * 2.f) + f.position.y;
			vertices.emplace_back(glm::vec3(x, y, 0.f), f.color, glm::vec2(0.5f, 0.5f));
		}
	}

	//compute area that should be visible:
	glm::vec2 scene_min = glm::vec2(-1.f, -1.f);
	glm::vec2 scene_max = glm::vec2(1.f, 1.f);

	//compute window aspect ratio:
	float aspect = drawable_size.x / float(drawable_size.y);

	//compute scale factor
	float scale = std::min(
		(0.05f * aspect) / (scene_max.x - scene_min.x), //... x must fit in [-aspect,aspect] ...
		(0.05f) / (scene_max.y - scene_min.y) //... y must fit in [-1,1].
	);

	//build matrix that scales and translates appropriately:
	glm::mat4 court_to_clip = glm::mat4(
		glm::vec4(scale / aspect, 0.f, 0.f, 0.f),
		glm::vec4(0.f, scale, 0.f, 0.f),
		glm::vec4(0.f, 0.f, 1.f, 0.f),
		glm::vec4(0.f, 0.f, 0.f, 1.f)
	);
	//NOTE: glm matrices are specified in *Column-Major* order,
	//so each line above is specifying a *column* of the matrix(!)

	//use alpha blending:
	glEnable(GL_BLEND);
	//don't use the depth test:
	glDisable(GL_DEPTH_TEST);

	//upload vertices to vertex_buffer:
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); //set vertex_buffer as current
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STREAM_DRAW); //upload vertices array
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//set color_texture_program as current program:
	glUseProgram(color_texture_program.program);

	//upload OBJECT_TO_CLIP to the proper uniform location:
	glUniformMatrix4fv(color_texture_program.OBJECT_TO_CLIP_mat4, 1, GL_FALSE, glm::value_ptr(court_to_clip));

	//use the mapping vertex_buffer_for_color_texture_program to fetch vertex data:
	glBindVertexArray(vertex_buffer_for_color_texture_program);

	//bind the solid white texture to location zero so things will be drawn just with their colors:
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, white_tex);

	//run the OpenGL pipeline:
	glDrawArrays(GL_TRIANGLES, 0, GLsizei(vertices.size()));

	//unbind the solid white texture:
	glBindTexture(GL_TEXTURE_2D, 0);

	//reset vertex array to none:
	glBindVertexArray(0);

	//reset current program to none:
	glUseProgram(0);

	GL_ERRORS(); //PARANOIA: print errors just in case we did something wrong.
}

void PlayMode::turn_cat() {
	cat_head->position.x = player.head_pos.x;
	cat_head->position.y = player.head_pos.y;

	cat_tail->position.x = player.tail_pos.x;
	cat_tail->position.y = player.tail_pos.y;
	/*
	if (!player.grabbing) {
		if (player.head_vel.x > 0.f && direction) { //using direction to avoid unnecessary writes to rotation
			cat_head->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
			direction = 0;
		}
		else if (player.head_vel.x < 0.f && !direction) {
			cat_head->rotation = glm::quat(0.0f, 0.0f, 1.0f, 0.0f);
			direction = 1;
		}
	}
	if (player.tail_vel.x > 0.f && tail_direction) {
		cat_tail->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		tail_direction = 0;
	}
	else if (player.tail_vel.x < 0.f && !tail_direction) {
		cat_tail->rotation = glm::quat(0.0f, 0.0f, 1.0f, 0.0f);
		tail_direction = 1;
	}
	*/

	// Quat lookat code, slightly adapted from this example
	// https://stackoverflow.com/questions/18172388/glm-quaternion-lookat-function
	glm::vec3 disp = cat_head->position - cat_tail->position;
	glm::vec3 norm_disp = glm::normalize(disp);
	glm::vec3 up(0.f, 1.f, 0.f);

	glm::quat body_rotation = glm::quatLookAt(norm_disp, up) * glm::quat(up * (3.14159f / 2));
	
	float dist = glm::distance(cat_head->position, cat_tail->position);
	cat_body->transform->position = (cat_head->position + cat_tail->position) * .5f;
	cat_body->transform->scale = glm::vec3(dist, 1.f, 1.f);


	// Is the normal up (nearly) parallel to direction?
    if(glm::abs(glm::dot(norm_disp, up)) < .9999f)
	if(glm::distance(disp, glm::vec3(0.f)) > .001f) {
		cat_head->rotation = body_rotation;
		cat_tail->rotation = body_rotation;
		cat_body->transform->rotation = body_rotation;
	}
}

void PlayMode::interact_objects(float elapsed) {
	// interaction with in-game objects
	for (uint8_t i = 0; i < fishes.size(); ++i) {
		Scene::Transform* fish = fishes[i];

		if (sense_counter < sense_SFX_cd) {
			// SFX cool down
			sense_counter += elapsed;

		}
		else {
			if (glm::abs(glm::length(cat_head->position - fish->position) - sensing_dist) < 1.0f) {
				// near fish, play nt effect
				// Don't play the sense sound effect, it's confusing
				//nt_SFX = Sound::play(*nt_effect_sample, 1.0f, 0.0f);

				// reset counter
				sense_counter = 0.0f;
			}
		}


		if (eat_counter < eat_SFX_cd) {
			// SFX cool down
			eat_counter += elapsed;

		}
		else {
			if (glm::abs(glm::length(cat_head->position - fish->position) - eat_dist) < 1.0f ||
				glm::abs(glm::length(cat_tail->position - fish->position) - eat_dist) < 1.0f) {
				// near fish, play nt effect
				cat_meow_SFX = Sound::play(*cat_meow_sample, 0.6f, 0.0f);

				// "delete" fish
				fish->scale = glm::vec3(0.0f, 0.0f, 0.0f);
				fishes.erase(fishes.begin() + i);
				i--;

				// increase player length
				player.max_length += 5.f;

				// reset counter
				eat_counter = 0.0f;
			}
		}

	}
}

void PlayMode::animation_update(float elapsed) {
	//spin fish
	spin_fish(elapsed);

	//reorient cat
	turn_cat();

	//peets
	animate_feet(elapsed);

	//update camera
	dynamic_camera.update(elapsed, (player.head_pos + player.tail_pos) / 2.f, space.pressed && player.grabbing, glm::distance(player.head_pos, player.tail_pos) / player.length);
}

//mostly taken from anne and georges game2
void PlayMode::animate_feet(float elapsed) {
	//slowly rotates through [0,1):
	//wobble += elapsed / 10.0f;
	//wobble -= std::floor(wobble);

	if (glm::length(player.tail_vel) > 0.1f) {
		wobblefront += elapsed * glm::length(player.tail_vel) / 100.0f;
		wobblefront -= std::floor(wobblefront);
		lbpeet->position = lbpeet_base + glm::vec3(
			-0.15f * std::sin((wobblefront + 5.0f) * 10.0f * 2.0f * float(M_PI)),
			-0.15f * std::sin((wobblefront + 5.0f) * 10.0f * 2.0f * float(M_PI)),
			0.0f
		);
		rbpeet->position = rbpeet_base + glm::vec3(
			0.15f * std::sin(wobblefront * 10.0f * 2.0f * float(M_PI)),
			-0.15f * std::sin(wobblefront * 10.0f * 2.0f * float(M_PI)),
			0.0f
		);
	}
	if (glm::length(player.head_vel) > 0.1f) {
		wobbleback += elapsed * glm::length(player.head_vel) / 100.0f;
		wobbleback -= std::floor(wobbleback);
		lfpeet->position = lfpeet_base + glm::vec3(
			-0.15f * std::sin(wobbleback * 10.0f * 2.0f * float(M_PI)),
			0.0f,//0.15f * std::sin(wobble * 10.0f * 2.0f * float(M_PI)),
			0.0f
		);
		rfpeet->position = rfpeet_base + glm::vec3(
			0.15f * std::sin(wobbleback * 10.0f * 2.0f * float(M_PI)),
			0.0f, //0.15f * std::sin(wobble * 10.0f * 2.0f * float(M_PI)),
			0.0f
		);
	}
}

void PlayMode::player_phys_update(float elapsed) {
	// Apply gravity
	player.head_vel.y -= elapsed * GRAVITY;
	player.tail_vel.y -= elapsed * GRAVITY;

	// Restrict stretching to only be allowed when we're grabbing a surface
	bool stretch_pressed = false;
	if(space.pressed) {
		stretch_pressed = player.grabbing;
	}

	player.length = stretch_pressed ? player.max_length : 1.f;

	float head_tail_dist = glm::distance(player.head_pos, player.tail_pos);

	if (stretch_pressed) {
		if(head_tail_dist > 4.f)
			stretched = true;
	}
	// If not pressing stretch, grabbing onto something, and the player has just recompressed, 
	// let go and apply the velocity to both halves
	else if (player.grabbing && stretched && head_tail_dist <= 4.f) {
		stretched = false;

		player.grabbing = false;
		player.head_grounded = false;
		player.head_vel += player.tail_vel *0.5f;
		player.tail_vel *= 0.5f;
	}

	player.bound_length = stretched || (stretch_pressed) ? player.max_length : 1.0f;

	do_auto_grab();

	player.movement(elapsed, left.pressed, right.pressed, up.pressed);

	// Do phyics update
	player.head_pos += player.head_vel * elapsed;
	player.tail_pos += player.tail_vel * elapsed;

	// Check collision with the walls and adjust the velocities accordingly
	player.collide_segments(collision_manager, 1.f, true);
	player.collide_segments(collision_manager, 1.f, false);

	// Air resistance only FIXED UPDATE
	timer += elapsed;
	while (timer > fixed_time) {
		player.head_vel *= 0.995f;
		player.tail_vel *= 0.995f;
		timer -= fixed_time;
	}
}