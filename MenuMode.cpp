#include "MenuMode.hpp"
#include "PlayMode.hpp"
#include "ControlMode.hpp"

#include "DrawLines.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"
#include "hex_dump.hpp"
#include "load_save_png.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>	//TODO: only for debug, can delete

#include <random>
#include <iostream>	//TODO: only for debug, delete later
	
MenuMode::MenuMode() {
	//taken from game0
	//----- allocate OpenGL resources -----
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

		//set up the vertex array object to describe arrays of Vertex:
		glVertexAttribPointer(
			color_texture_program->Position_vec4, //attribute
			3, //size
			GL_FLOAT, //type
			GL_FALSE, //normalized
			sizeof(Vertex), //stride
			(GLbyte*)0 + 0 //offset
		);
		glEnableVertexAttribArray(color_texture_program->Position_vec4);
		//[Note that it is okay to bind a vec3 input to a vec4 attribute -- the w component will be filled with 1.0 automatically]

		glVertexAttribPointer(
			color_texture_program->Color_vec4, //attribute
			4, //size
			GL_UNSIGNED_BYTE, //type
			GL_TRUE, //normalized
			sizeof(Vertex), //stride
			(GLbyte*)0 + 4 * 3 //offset
		);
		glEnableVertexAttribArray(color_texture_program->Color_vec4);

		glVertexAttribPointer(
			color_texture_program->TexCoord_vec2, //attribute
			2, //size
			GL_FLOAT, //type
			GL_FALSE, //normalized
			sizeof(Vertex), //stride
			(GLbyte*)0 + 4 * 3 + 4 * 1 //offset
		);
		glEnableVertexAttribArray(color_texture_program->TexCoord_vec2);

		//done referring to vertex_buffer, so unbind it:
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//done setting up vertex array object, so unbind it:
		glBindVertexArray(0);

		GL_ERRORS(); //PARANOIA: print out any OpenGL errors that may have happened
	}
	/*
	{ //solid white texture:
		//ask OpenGL to fill white_tex with the name of an unused texture object:
		glGenTextures(1, &white_tex);

		//bind that texture object as a GL_TEXTURE_2D-type texture:
		glBindTexture(GL_TEXTURE_2D, white_tex);

		//upload a 1x1 image of solid white to the texture:
		glm::uvec2 size = glm::uvec2(1, 1);
		std::vector< glm::u8vec4 > data(size.x * size.y, glm::u8vec4(0x00, 0x00, 0x00, 0xff)); //whoops actually black tex
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
	}*/

	std::vector< glm::u8vec4 > data;
	glm::uvec2 size(0, 0);

	load_png(data_path(bg_play_path), &size, &data, UpperLeftOrigin);
	glGenTextures(1, &play_highlighted_tex);
	glBindTexture(GL_TEXTURE_2D, play_highlighted_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	data.clear();

	load_png(data_path(bg_control_path), &size, &data, UpperLeftOrigin);
	glGenTextures(1, &control_highlighted_tex);
	glBindTexture(GL_TEXTURE_2D, control_highlighted_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	data.clear();

	load_png(data_path(bg_path), &size, &data, UpperLeftOrigin);
	glGenTextures(1, &bg_tex);
	glBindTexture(GL_TEXTURE_2D, bg_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	data.clear();

	//glBindVertexArray(0);

	active_tex = bg_tex;
}

MenuMode::~MenuMode() {
	glDeleteTextures(1, &white_tex);
	white_tex = 0;

	glDeleteTextures(1, &bg_tex);
	bg_tex = 0;

	glDeleteTextures(1, &play_highlighted_tex);
	play_highlighted_tex = 0;

	glDeleteTextures(1, &control_highlighted_tex);
	control_highlighted_tex = 0;
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
	if(mouse_pos.x >= -0.93f && mouse_pos.x <= -0.39f){
		if(mouse_pos.y <= 0.05f && mouse_pos.y >= -0.22f){
			//highlight_pos.y = -0.05f;
			active_tex = play_highlighted_tex;
			modeSelect = 'p';	// play mode
		}else if(mouse_pos.y <= -0.42f && mouse_pos.y >= -0.69f){
			//highlight_pos.y = -0.2f;
			active_tex = control_highlighted_tex;
			modeSelect = 'c';	// control prompt
		}
		else {
			active_tex = bg_tex;
			modeSelect = 'm';
		}
	}
	else {
		active_tex = bg_tex;
		modeSelect = 'm';
	}

	if(clicked){
		switch(modeSelect){
			case 'p':
				Mode::set_current(std::make_shared< PlayMode >(0));
				break;

			case 'c':
				Mode::set_current(std::make_shared< ControlMode >());
				break;

			default:
				break;
		}

		clicked = false;

	}
	

}

void MenuMode::draw(glm::uvec2 const &drawable_size) {
	//clear the color buffer:
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//use alpha blending:
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//don't use the depth test:
	glDisable(GL_DEPTH_TEST);

	projection = glm::ortho(0.0f, float(drawable_size.x), 0.0f, float(drawable_size.y));
	glUseProgram(color_texture_program->program);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vertex_buffer_for_color_texture_program);
	glUniformMatrix4fv(color_texture_program->OBJECT_TO_CLIP_mat4, 1, GL_FALSE, glm::value_ptr(projection));

	draw_image(active_tex, 0.0f, (float)drawable_size.x, 0.0f, (float)drawable_size.y);

	/*
	{ //use DrawLines to overlay some text:
		//draw button and text
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
				white);
			float ofs = 2.0f / drawable_size.y;
			lines.draw_text(text,
				glm::vec3(at.x + ofs, at.y + ofs, 0.0),
				glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
				white);			
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
	*/

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	GL_ERRORS();
}

void MenuMode::draw_image(GLuint &tex, float left, float right, float top, float bottom) {
	std::vector< Vertex > vertices;
	vertices.emplace_back(Vertex(glm::vec3(left, top, 0.0f), white, glm::vec2(0.0f, 1.0f)));
	vertices.emplace_back(Vertex(glm::vec3(right, top, 0.0f), white, glm::vec2(1.0f, 1.0f)));
	vertices.emplace_back(Vertex(glm::vec3(left, bottom, 0.0f), white, glm::vec2(0.0f, 0.0f)));

	vertices.emplace_back(Vertex(glm::vec3(left, bottom, 0.0f), white, glm::vec2(0.0f, 0.0f)));
	vertices.emplace_back(Vertex(glm::vec3(right, top, 0.0f), white, glm::vec2(1.0f, 1.0f)));
	vertices.emplace_back(Vertex(glm::vec3(right, bottom, 0.0f), white, glm::vec2(1.0f, 0.0f)));

	glBindTexture(GL_TEXTURE_2D, tex);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STREAM_DRAW); //upload vertices array
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	GL_ERRORS();
}