#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"


#include <glm/glm.hpp>

#include <vector>
#include <deque>

#define PLAYER_SPEED 10.f
#define JUMP_SPEED 10.f
#define CAMERA_SPEED 9.f
#define GRAVITY 10.f
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
		bool reached;
		glm::vec2 position;

		checkpoint() : reached{false}, position{glm::vec2(0.f)} {}
		checkpoint(glm::vec2 position_) : reached{false}, position{position_} {}

		bool operator < (const checkpoint& c) const {
			return position.x < c.position.x;
		}
	};

	//----- helper functions ----- see Trello documentation for details
	//collisions
	std::vector<intersection> get_collisions(const circle &c, const std::vector<line_segment> &ls);
	intersection get_capsule_collision(const circle &c, const line_segment &l, bool &is_hit);
	std::vector<line_segment> get_lines(const Scene::Transform* platform);

	//camera behavior
	void update_camera(float elapsed);

	//checkpoint behavior
	void sort_checkpoints();
	void update_checkpoint();

	//----- movement updates -----
	void collide_segments(glm::vec2 &pos, glm::vec2 &vel, float radius, bool &grounded);
	bool grab_ledge(glm::vec2& pos, float radius);
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

	glm::vec2 head_pos;
	glm::vec2 head_vel;
	glm::vec2 tail_pos;
	glm::vec2 tail_vel;

	bool head_grounded;
	bool tail_grounded;

	// Indicates if the player has extended the player size, and the player hasn't yet compressed back to actual size
	bool stretched = false;

	float playerlength = 5.f; //length of spring
	float k = 20.f; //spring constant
	float grab_radius = 0.5f; //grabbable radius

	//camera motion
	glm::vec3 camera_pos;
	float camera_default_z = 50.f;
	float camera_zoomed_out = 1.f;
	
	//scene
	Scene scene;
	Scene::Camera *camera = nullptr;
	
	//scene objects
	std::vector<Scene::Transform*> platforms;
		
	Scene::Transform* doughnut = nullptr;
	Scene::Transform* cat_head = nullptr;
	Scene::Transform* cat_tail = nullptr;

	//sound
	std::shared_ptr< Sound::PlayingSample > bgm_loop;
};
