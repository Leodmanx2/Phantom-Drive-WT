#define GLFW_INCLUDE_NONE

#include <iostream>

#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Log.h>

#include "RenderComponent.hpp"
#include "Renderer.hpp"

using namespace plog;
using namespace std;
using namespace glm;

void glfw_error_callback(int, const char* description) {
	LOG(error) << description;
}

int main() {
	RollingFileAppender<TxtFormatter>  fileAppender("log/log.txt", 1000000, 2);
	ColorConsoleAppender<TxtFormatter> consoleAppender;
	plog::init(debug, &fileAppender).addAppender(&consoleAppender);
	LOG(info) << "starting program";

	try {
		glfwSetErrorCallback(glfw_error_callback);
		if(!glfwInit()) {
			LOG(fatal) << "GLFW initialization failed";
			return EXIT_FAILURE;
		}

		const unsigned int width  = 640;
		const unsigned int height = 480;

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		GLFWwindow* window =
		  glfwCreateWindow(width, height, "Render Test", nullptr, nullptr);
		if(!window) {
			cerr << "Window or OpenGL context could not be created\n";
			glfwTerminate();
			return EXIT_FAILURE;
		}
		glfwMakeContextCurrent(window);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		globjects::init(glfwGetProcAddress);

		// Enable v-sync
		glfwSwapInterval(1);

		Renderer renderer(width, height);

		vector<Light> lights;
		Light         light(
      {0, 0, 0}, {0, 0, 0}, {255.0f, 255.0f, 255.0f}, 1.0f, 360.0f, 100.0f);
		lights.push_back(light);

		RenderComponent component("ass/Models/Akari/diffuse.dds",
		                          "ass/Models/Akari/specular.dds",
		                          "test_shader",
		                          "ass/Models/Akari/model.mdl");

		mat4 model(1.0f);

		vec4 eye(1.0f, 1.0f, 1.0f, 1.0f);
		mat4 view = lookAt(vec3{eye}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f});

		mat4 projection =
		  ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));

		RenderTask task(component, 0, model, view, projection, eye, 1.0f, lights);

		LOG(debug) << "starting main loop";

		glfwPollEvents();
		while(!glfwWindowShouldClose(window)) {
			renderer.queue(task);
			renderer.draw();
		}

		LOG(debug) << "loop finished";

		glfwTerminate();
	} catch(const std::exception& exception) {
		LOG(fatal) << exception.what();
		LOG(info) << "exited program unsuccessfully";
		return EXIT_FAILURE;
	}
}
