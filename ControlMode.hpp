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
	glm::u8vec4 white = glm::u8vec4(255, 255, 255, 255);

	//background
	GLuint white_tex;
	GLuint bg1_tex;
	GLuint bg2_tex;
	GLuint bg1_back_highlighted_tex;
	GLuint bg1_next_highlighted_tex;
	GLuint bg2_back_highlighted_tex;
	std::string bg1_path = "control_bg1.png";
	std::string bg2_path = "control_bg2.png";
	std::string bg1_back_path = "control_bg1_backselected.png";
	std::string bg1_next_path = "control_bg1_nextselected.png";
	std::string bg2_back_path = "control_bg2_backselected.png";
	GLuint active_tex;

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