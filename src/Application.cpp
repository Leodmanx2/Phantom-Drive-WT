#include "Application.hpp"

#define GLFW_INCLUDE_NONE

#include "RenderContext.hpp"
#include "Renderer.hpp"
#include "ShaderProgram.hpp"

#include <GLFW/glfw3.h>
#include <glbinding\gl\gl.h>
#include <globjects/globjects.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Log.h>

using namespace plog;
using std::make_shared;
using std::make_unique;
using namespace gl;
using std::move;

const unsigned int INIT_WIDTH  = 640;
const unsigned int INIT_HEIGHT = 480;

void key_callback(GLFWwindow* window, int key, int, int action, int modifiers) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void mouse_button_callback(GLFWwindow*, int button, int action, int modifiers) {
}

void cursor_position_callback(GLFWwindow*, double xpos, double ypos) {}

void glfw_error_callback(int, const char* description) {
	LOG(error) << description;
}

int main(int argc, char** argv) {
	// Initialize plog
	RollingFileAppender<TxtFormatter>  fileAppender("log/log.txt", 1000000, 2);
	ColorConsoleAppender<TxtFormatter> consoleAppender;
	plog::init(debug, &fileAppender).addAppender(&consoleAppender);

	// Initialize glfw
	glfwSetErrorCallback(glfw_error_callback);
	if(!glfwInit()) {
		LOG(fatal) << "GLFW initialization failed";
		return EXIT_FAILURE;
	}

	// Make window
#ifdef DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(
	  INIT_WIDTH, INIT_HEIGHT, "Phantom Drive (WT)", nullptr, nullptr);
	if(!window) {
		LOG(fatal) << "Window or OpenGL context could not be created";
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Enable v-sync
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	// Initialize globjects
	globjects::init(glfwGetProcAddress);
	globjects::setCurrentContext();

	// Initialize rendering pipeline
	auto frame_buffer = PD::init_framebuffer(INIT_WIDTH, INIT_HEIGHT);

	auto vertex_shader = make_shared<VertexShaderProgram>("sample_vs.glsl");
	auto ambient_shader =
	  make_shared<FragmentShaderProgram>("sample_ambient_fs.glsl");
	auto highlight_shader =
	  make_shared<FragmentShaderProgram>("sample_highlight_fs.glsl");

	auto ambient_pipeline =
	  make_unique<PD::ShaderPipeline>(vertex_shader, ambient_shader);

	auto highlight_pipeline =
	  make_unique<PD::ShaderPipeline>(vertex_shader, highlight_shader);

	auto context = make_unique<PD::RenderContext>(
	  move(frame_buffer), move(ambient_pipeline), move(highlight_pipeline));

	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//TODO: Do stuff
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
}
