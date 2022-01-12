#include "Application.hpp"

#define GLFW_INCLUDE_NONE

#include "Renderer.hpp"

#include <GLFW/glfw3.h>
#include <globjects/globjects.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Log.h>

using namespace plog;

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

	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//TODO: Do stuff
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
}
