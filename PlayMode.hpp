#include "Mode.hpp"

#include "Connection.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode(Client &client);
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//helper functions and typedefs
	typedef std::pair<glm::vec2, glm::vec2> line_segment;	//defined as two endpoints
	typedef std::pair<glm::vec2, float> circle;				//defined as a center position and a radius

	/*
	Function: get_collisions
	Input: circle c, vector of line segments ls
	Output: vector of pairs of positions and normals corresponding to line segments which collide
	*/
	std::vector<std::pair<glm::vec2, glm::vec2>> get_collisions(circle c, std::vector<line_segment> ls);

	//----- game state -----

	std::vector<line_segment> line_segments;

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	//last message from server:
	std::string server_message;

	//connection to server:
	Client &client;

};
