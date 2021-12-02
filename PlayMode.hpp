#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"

#include "ColorTextureProgram.hpp"

#include "Collisions.hpp"
#include "DynamicCamera.hpp"
#include "Player.hpp"
#include "KeyPoint.hpp"
#include "load_texture.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>
#include <random>

#define GRAVITY 10.f
#define DEATH_BOUND -50.f
#define LEVEL_CNT 4	// 3

struct PlayMode : Mode {
	PlayMode(int level);
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- helper structs -----
	struct firework {
		float age;
		float speed;
		glm::vec2 position;
		glm::vec2 direction;
		glm::u8vec4 color;

		firework(float speed_, glm::vec2 position_)
		: age{0.f}, speed{speed_}, position{position_} {
			//we want the firework to be in an upwards position so choose a random upwards direction
			//https://stackoverflow.com/questions/686353/random-float-number-generation
			float y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) + 0.1f; //random number between 0.1f and 1.1f
			float x = 2.f * static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 1.f; //another random number between 0.f and 1.f

			direction = glm::normalize(glm::vec2(x, y));

			//now we want to randomize the color of the firework
			color = glm::u8vec4(rand() % 256, rand() % 256, rand() % 256, 255);
		}

		void update(float elapsed) {
			age += elapsed;
			//direction.y -= elapsed * GRAVITY;
			position += elapsed * speed * direction;
		}
	};

	//----- helper functions ----- see Trello documentation for details

	void player_phys_update(float elapsed);
	void animation_update(float elapsed);
	void interact_objects(float elapsed);

	//fish behavior
	void spin_fish(float elapsed);

	//end game behavior
	void celebrate_update(float elapsed);
	void celebrate_draw(glm::uvec2 const &drawable_size);

	//----- movement updates -----
	void do_auto_grab();
	void turn_cat();
	void animate_feet(float elapsed);
	float timer = 0.0f;
	float fixed_time = 0.015f;

	//----- game state -----
	bool game_over = false;
	int level_id;
	float level_switch_timer = 0.f;
	bool skip_level = false;

	//collisions
	CollisionManager collision_manager;

	//dynamic camera
	DynamicCamera dynamic_camera;

	//checkpoints:
	std::vector<Check_Point> checkpoints; //checkpoints should be sorted by x coordinate
	long unsigned int curr_checkpoint_id;

	//grab points
	std::vector<Grab_Point> grab_points;

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, space;

	//player struct
	Player player;

	// Player motion
	uint8_t direction = 0; //facing right
	uint8_t tail_direction = 0;

	// Indicates if the player has extended the player size, and the player hasn't yet compressed back to actual size
	bool stretched = false;

	float eat_dist = 3.0f;		// distance range that consider hit object
	float sensing_dist = 20.0f;	// sensing distance, should be further than eat_dist
	bool isNearFish = false;	// flag for near fish

	float sense_counter = 0.0f;
	float eat_counter = 0.0f;
	float sense_SFX_cd = 5.0f;	// cool down for SFX, sense effect take around 2.0 sec
	float eat_SFX_cd = 3.0f;

	//fireworks
	size_t max_fireworks_num = 30;
	float max_fireworks_age = 3.f;
	float fireworks_time_between = 0.1f;
	float fireworks_countdown = 0.f;
	float fireworks_speed = 20.f;
	std::vector<firework> fireworks;
	
	//scene
	Scene scene;
	Scene::Camera *camera = nullptr;
	
	//scene objects
	std::vector<Scene::Transform*> platforms;
	std::vector<Scene::Transform*> fishes;
		
	Scene::Transform* doughnut = nullptr;
	Scene::Transform* cat_head = nullptr;
	Scene::Transform* cat_tail = nullptr;
	Scene::Drawable* cat_body = nullptr;

	//feet, stolen from my game2
	Scene::Transform* lbpeet = nullptr;
	Scene::Transform* lfpeet = nullptr;
	Scene::Transform* rbpeet = nullptr;
	Scene::Transform* rfpeet = nullptr;
	glm::vec3 lfpeet_base;
	glm::vec3 lbpeet_base;
	glm::vec3 rbpeet_base;
	glm::vec3 rfpeet_base;
	float wobblefront = 0.0f;
	float wobbleback = 0.0f;

	std::string bg1_path = "bg.png";
	std::string bg2_path = "bg2.png";
	std::string bg3_path = "bg3.png";
	std::string bg4_path = "bg4.png";
	std::string current_bg = bg1_path;

	//sound
	std::shared_ptr< Sound::PlayingSample > bgm_loop;
	std::shared_ptr< Sound::PlayingSample > spring_boing_SFX;
	std::shared_ptr< Sound::PlayingSample > cat_meow_SFX;		// play when get to fish
	std::shared_ptr< Sound::PlayingSample > cat_scream_SFX;		// play when fall from platform
	std::shared_ptr< Sound::PlayingSample > nt_SFX;				// play when near fish

	//----- opengl assets / helpers ------ from Game 0
	//draw functions will work on vectors of vertices, defined as follows:
	struct Vertex {
		Vertex(glm::vec3 const &Position_, glm::u8vec4 const &Color_, glm::vec2 const &TexCoord_) :
			Position(Position_), Color(Color_), TexCoord(TexCoord_) { }
		glm::vec3 Position;
		glm::u8vec4 Color;
		glm::vec2 TexCoord;
	};
	static_assert(sizeof(Vertex) == 4*3 + 1*4 + 4*2, "PlayMode::Vertex should be packed");

	//Shader program that draws transformed, vertices tinted with vertex colors:
	ColorTextureProgram color_texture_program;

	//Buffer used to hold vertex data during drawing:
	GLuint vertex_buffer = 0;

	//Vertex Array Object that maps buffer locations to color_texture_program attribute locations:
	GLuint vertex_buffer_for_color_texture_program = 0;

	//Solid white texture:
	GLuint white_tex = 0;
	
	//----- opengl assets / helpers ------
	
	struct Vertex2 {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::u8vec4 Color;
		glm::vec2 TexCoord;
	};
	static_assert(sizeof(Vertex2) == 3*4+3*4+4*1+2*4, "Vertex2 is packed.");
};
