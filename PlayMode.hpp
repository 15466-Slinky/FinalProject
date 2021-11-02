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
	typedef std::pair<glm::vec2, glm::vec2> line_segment; //defined as two endpoints
	typedef std::pair<glm::vec2, float> circle; //defined as a center position and a radius
	typedef std::pair<glm::vec2, glm::vec2> intersection; //defined as a point of intersection and a surface normal vector

	/*
	Function: get_collisions
	Input: circle c, vector of line segments ls
	Output: vector of pairs of positions and normals corresponding to line segments which collide
	*/
	std::vector<intersection> get_collisions(circle c, std::vector<line_segment> ls);

	//----- game state -----

	std::vector<line_segment> line_segments;

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	// Player motion
	glm::vec2 head_pos;
	glm::vec2 head_vel;
	glm::vec2 tail_pos;
	glm::vec2 tail_vel;

	
	//scene
	Scene scene;
	Scene::Camera *camera = nullptr;
	
	//scene objects
	std::vector<Scene::Transform*> platforms;
		
	Scene::Transform* doughnut = nullptr;
	Scene::Transform* cat_head = nullptr;
	Scene::Transform* cat_tail = nullptr;
	
	/*
	Function: get_upper_line
	Input: pointer to platform
	Output: pairs of two end points of the platform's uppersurface line
	*/
	line_segment get_upper_line(Scene::Transform* platform);
	
};
