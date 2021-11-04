#include "Mode.hpp"

#include "Scene.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

#define PLAYER_SPEED 10.f

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//helper functions and typedefs
	struct line_segment {
		glm::vec2 ep1;
		glm::vec2 ep2;

		line_segment(glm::vec2 ep1_, glm::vec2 ep2_) {
			assert(ep1_ != ep2); //we do not allow degenerate line segments
			ep1 = ep1_;
			ep2 = ep2_;
		}
	};

	struct circle {
		glm::vec2 center;
		float radius;

		circle(glm::vec2 center_, float radius_) {
			assert(radius_ != 0.0f); //we do not allow degenerate circles
			center = center_;
			radius = radius_;
		}
	};

	struct intersection {
		glm::vec2 point_of_intersection;
		glm::vec2 surface_normal;

		intersection(glm::vec2 point_of_intersection_, glm::vec2 surface_normal_) {
			point_of_intersection = point_of_intersection_;
			surface_normal = surface_normal_;
		}
	};

	/*
	Function: get_collisions
	Input: circle c, vector of line segments ls
	Output: vector of intersections corresponding to line segments which collide
	*/
	std::vector<intersection> get_collisions(circle c, std::vector<line_segment> ls);

	/*
	Function: get_capsule_collision
	Input: circle c, line segment l
	Output: an "intersection" where the point of intersection is the closest exterior point instead.
			if we do not collide with the capsule zone, then the point of intersection
			and the surface normals are vectors of 0.0f
	*/
	intersection get_capsule_collision(circle c, line_segment l);

	//----- movement updates -----
	void free_movement(float elapsed);
	void fixed_head_movement(float elapsed);
	void fixed_tail_movement(float elapsed);

	//----- game state -----
	std::vector<line_segment> line_segments;

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;
	bool fixed_head = false;
	bool fixed_tail = false;

	// Player motion
	glm::vec2 head_pos;
	glm::vec2 head_vel;
	glm::vec2 tail_pos;
	glm::vec2 tail_vel;
	float playerlength = 5.0f; //length of spring
	float k = 4.0f; //spring constant
	
	//scene
	Scene scene;
	Scene::Camera *camera = nullptr;
	
	//scene objects
	std::vector<Scene::Transform*> platforms;
		
	Scene::Transform* doughnut = nullptr;
	Scene::Transform* cat_head = nullptr;
	Scene::Transform* cat_tail = nullptr;
	
	/*
	Function: get_lines
	Input: pointer to the platform
	Output: line segment vector: {left line, right line, upper line, bottom line}
	*/
	std::vector<line_segment> get_lines(Scene::Transform* platform);
	
};
