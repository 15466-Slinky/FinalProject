#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"


#include <glm/glm.hpp>

#include <vector>
#include <deque>

#define PLAYER_SPEED 10.f
#define JUMP_SPEED 20.f
#define CAMERA_SPEED 9.f
#define GRAVITY 10.f
#define GRAB_RADIUS 1.5f
#define DEATH_BOUND -50.f

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- helper structs -----
	struct line_segment {
		glm::vec2 ep1;
		glm::vec2 ep2;
		glm::vec2 surface_normal;

		line_segment(glm::vec2 ep1_, glm::vec2 ep2_, glm::vec2 surface_normal_) 
		: ep1{ep1_}, ep2{ep2_},
		  surface_normal{glm::normalize(surface_normal_)} {
			assert(ep1 != ep2); //we do not allow degenerate line segments
			assert(surface_normal != glm::vec2(0.f)); //do not allow zero-vector to be a surface normal
		}
	};
	struct circle {
		glm::vec2 center;
		float radius;

		circle(glm::vec2 center_, float radius_) : center{center_}, radius{radius_} {
			assert(radius_ != 0.f); //we do not allow degenerate circles
		}
	};
	struct intersection {
		glm::vec2 point_of_intersection;
		glm::vec2 surface_normal;

		intersection(glm::vec2 point_of_intersection_, glm::vec2 surface_normal_)
		: point_of_intersection{point_of_intersection_},
		  surface_normal{glm::normalize(surface_normal_)} {}
	};
	struct checkpoint {
		std::string name;
		bool reached;
		glm::vec2 position;

		Scene::Transform* box_front;
		Scene::Transform* box_left;
		Scene::Transform* box_right;

		checkpoint() : name{"Unnamed"}, reached{false}, position{glm::vec2(0.f)},
			box_front{nullptr}, box_left{nullptr}, box_right{nullptr} {}
		checkpoint(std::string name_, glm::vec2 position_) : name{name_}, reached{false}, position{position_},
			box_front{nullptr}, box_left{nullptr}, box_right{nullptr} {}

		bool box_has_sides() {
			return (box_left != nullptr && box_right != nullptr && box_front != nullptr);
		}
		bool operator < (const checkpoint& c) const {
			return position.x < c.position.x;
		}
	};

	struct Grab_Point {
		glm::vec2 position;
		float past_player_dist;
		
		Grab_Point(glm::vec2 position_) : position{position_} {
			past_player_dist = 0;
		}
	};


	//----- helper functions ----- see Trello documentation for details
	//collisions
	std::vector<intersection> get_collisions(const circle &c, const std::vector<line_segment> &ls);
	intersection get_capsule_collision(const circle &c, const line_segment &l, bool &is_hit);
	std::vector<line_segment> get_lines(const Scene::Transform* platform);

	GLuint load_texture(std::string filename);
	void player_phys_update(float elapsed);
	void animation_update(float elapsed);

	//camera behavior
	void update_camera(float elapsed);

	//checkpoint behavior
	void sort_checkpoints();
	void update_checkpoints();
	void activate_checkpoint(int checkpoint_id, float elapsed);
	bool checkpoint_find_sides(checkpoint* c);
	bool activating_checkpoint = false;
	float accumulated_time = 0.f;

	//fish behavior
	void spin_fish(float elapsed);

	//----- movement updates -----
	void collide_segments(glm::vec2 &pos, glm::vec2 &vel, float radius, bool &grounded);
	bool grab_ledge(glm::vec2& pos, float radius);
	void do_auto_grab();
	void free_movement(float elapsed);
	void fixed_head_movement(float elapsed);
	void fixed_tail_movement(float elapsed);
	void respawn();

	//----- game state -----
	//terrain:
	std::vector<line_segment> line_segments; //for platforms

	//checkpoints:
	std::vector<checkpoint> checkpoints; //checkpoints should be sorted by x coordinate
	int curr_checkpoint_id;
	checkpoint curr_checkpoint;
	checkpoint next_checkpoint;

	std::vector<Grab_Point> grab_points;

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, space;
	bool fixed_head = false;
	bool fixed_tail = false;

	// Player motion
	glm::vec2 head_start;
	glm::vec2 tail_start;

	glm::vec2 head_pos, tail_pos;
	glm::vec2 head_vel, tail_vel;

	bool head_grounded;
	bool tail_grounded;

	// Indicates if the player has extended the player size, and the player hasn't yet compressed back to actual size
	bool stretched = false;

	float playerlength = 5.f; //length of spring
	float k = 20.f; //spring constant
	float grab_radius = 0.5f; //grabbable radius

	float eat_dist = 3.0f;		// distance range that consider hit object
	float sensing_dist = 20.0f;	// sensing distance, should be further than eat_dist
	bool isNearFish = false;	// flag for near fish

	float sense_counter = 0.0f;
	float eat_counter = 0.0f;
	float sense_SFX_cd = 5.0f;	// cool down for SFX, sense effect take around 2.0 sec
	float eat_SFX_cd = 3.0f;


	//camera motion
	glm::vec3 camera_pos;
	float camera_default_z = 50.f;
	float camera_zoomed_out = 1.f;
	
	//scene
	Scene scene;
	Scene::Camera *camera = nullptr;
	
	//scene objects
	std::vector<Scene::Transform*> platforms;
	std::vector<Scene::Transform*> fishes;
		
	Scene::Transform* doughnut = nullptr;
	Scene::Transform* cat_head = nullptr;
	Scene::Transform* cat_tail = nullptr;

	//sound
	std::shared_ptr< Sound::PlayingSample > bgm_loop;
	std::shared_ptr< Sound::PlayingSample > spring_boing_SFX;
	std::shared_ptr< Sound::PlayingSample > cat_meow_SFX;		// play when get to fish
	std::shared_ptr< Sound::PlayingSample > cat_scream_SFX;		// play when fall from platform
	std::shared_ptr< Sound::PlayingSample > nt_SFX;				// play when near fish
};
