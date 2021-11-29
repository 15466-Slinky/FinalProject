#include "Mode.hpp"

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "LitColorTextureProgram.hpp"
#include "ColorTextureProgram.hpp"
#include "GL.hpp"

#include <string>
#include <vector>
#include <deque>

struct ControlMode : Mode {
	ControlMode();
	virtual ~ControlMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	void draw_image(GLuint &tex, float left, float right, float top, float bottom);

	//----- game state -----

	//input tracking:
	bool clicked = false;
	char modeSelect = 'c';	// record selected mode, ('m', 'p', 'c') for menu, play, control
	glm::vec2 mouse_pos = glm::vec2(std::numeric_limits<float>::max(),
									std::numeric_limits<float>::max());	// mouse current position respect to window, origin at middle, [-1,1]

	int menu_select = 0;

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
	glm::u8vec4 white = glm::u8vec4(255, 255, 255, 255);
	glm::u8vec4 font_color = glm::u8vec4(0x50, 0x44, 0x3b, 0xff);

	//background
	GLuint white_tex;
	GLuint bg_tex;
	std::string bg_path = "control_bg.png";

	struct Vertex {
		Vertex(glm::vec3 const& Position_, glm::u8vec4 const& Color_, glm::vec2 const& TexCoord_) :
			Position(Position_), Color(Color_), TexCoord(TexCoord_) { }
		glm::vec3 Position;
		glm::u8vec4 Color;
		glm::vec2 TexCoord;
	};
	static_assert(sizeof(Vertex) == 4 * 3 + 1 * 4 + 4 * 2, "PlayMode::Vertex should be packed");

	glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f);

	//Buffer used to hold vertex data during drawing:
	GLuint vertex_buffer = 0;
	GLuint VAO = 0; //text

	//Vertex Array Object that maps buffer locations to color_texture_program attribute locations:
	GLuint vertex_buffer_for_color_texture_program = 0;
	GLuint VBO = 0; //text
};