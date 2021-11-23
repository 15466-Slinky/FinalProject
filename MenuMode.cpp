#include "MenuMode.hpp"
#include "PlayMode.hpp"


#include "DrawLines.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"
#include "hex_dump.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>	//TODO: only for debug, can delete


#include <random>
#include <iostream>	//TODO: only for debug, delete later
	
MenuMode::MenuMode() {
}

MenuMode::~MenuMode() {
}


bool MenuMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	// check where is mouse and which button to be clicked

	if (evt.type == SDL_MOUSEBUTTONDOWN) {
		clicked = true;

		return true;
	} else if (evt.type == SDL_MOUSEMOTION) {
		// record current location, to hightlight option
		mouse_pos = glm::vec2(evt.motion.x, evt.motion.y);
		mouse_pos.x = 2.0f * (mouse_pos.x / float(window_size.x)) - 1.0f;
		mouse_pos.y = -1.0f * (2.0f * (mouse_pos.y / float(window_size.y)) - 1.0f);
		return true;
	}

	return false;
}

void MenuMode::update(float elapsed) {

	//std::cout << "mouse_pos: " << glm::to_string(mouse_pos) << std::endl;

	// update highlight
	if(mouse_pos.x <= 0.0f){
		if(mouse_pos.y >= -0.1f){
			highlight_pos.y = -0.05f;
			modeSelect = 'p';	// play mode
		}else{
			highlight_pos.y = -0.2f;
			modeSelect = 'c';	// control prompt
		}
	}

	if(clicked){
		switch(modeSelect){
			case 'p':
				Mode::set_current(std::make_shared< PlayMode >());
				break;

			case 'c':
				//TODO: show control prompt, or put stage switch here
				//std::cout << "not implemented" << std::endl;
				break;

			default:
				break;
		}

		clicked = false;

	}
	

}

void MenuMode::draw(glm::uvec2 const &drawable_size) {
	// draw button and text

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	{ //use DrawLines to overlay some text:
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
				glm::u8vec4(0x00, 0x00, 0x00, 0x00));
			float ofs = 2.0f / drawable_size.y;
			lines.draw_text(text,
				glm::vec3(at.x + ofs, at.y + ofs, 0.0),
				glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
				glm::u8vec4(0xff, 0xff, 0xff, 0x00));			
		};

		glm::vec2 left_anchor = glm::vec2(-aspect + 0.1f, 0.0f);

		// draw game title
		draw_text(left_anchor + title_pos, "Slinky Game", 0.4f);

		// draw menu buttons
		draw_text(left_anchor + start_button_pos, "Start Game", 0.1f);	// start button
		draw_text(left_anchor + control_button_pos, "Controls", 0.1f);	// control prompt button

		//prompt
		draw_text(glm::vec2(-aspect + 0.1f,-0.8f), "Select with mouse", 0.09f);

		// credits
		draw_text(glm::vec2(-aspect + 0.1f,-0.9f),
				"Produced by Slinky Group: Anne He, George Ralph, Katherine Wang, Wenxuan Ou, and Tejas Srivatsav",
				0.05f);

		// highlight box
		lines.draw_box(glm::mat4(
				0.21f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.09f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				-aspect + highlight_pos.x, highlight_pos.y, 0.0f, 1.0f
			),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00)		// color
		);

		
	}

	


	GL_ERRORS();
	

}

