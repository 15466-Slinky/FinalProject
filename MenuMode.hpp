#include "Mode.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct MenuMode : Mode {
	MenuMode();
	virtual ~MenuMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	glm::vec2 clicked_pos = glm::vec2(std::numeric_limits<float>::max(),
									std::numeric_limits<float>::max());	// mouse click position, respect to window, origin at middle, [-1,1]

	glm::vec2 mouse_pos = glm::vec2(std::numeric_limits<float>::max(),
									std::numeric_limits<float>::max());	// mouse current position, respect to window

	int menu_select = 0;


	//title position
	glm::vec2 title_pos = glm::vec2(0.0f, 0.3f);

	//button positions
	glm::vec2 start_button_pos = glm::vec2(0.0f, -0.1f); //TODO: set button positions
	glm::vec2 control_button_pos = glm::vec2(0.0f, -0.25f); //TODO: set button 

	glm::vec2 highlight_pos = glm::vec2(0.3f, -0.05f);	// overlay menu options, (0.3f, -0.05f) or (0.3f, -0.1)
	glm::mat4 hightlight_transform = glm::mat4(
				0.21f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.09f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
};