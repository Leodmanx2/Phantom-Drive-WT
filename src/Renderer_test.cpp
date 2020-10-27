#define GLFW_INCLUDE_NONE

#include <cmath>
#include <iostream>

#include <GLFW/glfw3.h>
#include <glbinding-aux/Meta.h>
#include <glbinding-aux/types_to_string.h>
#include <glbinding/AbstractFunction.h>
#include <glbinding/FunctionCall.h>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Log.h>
#include <sstream>

#include "RenderComponent.hpp"
#include "Renderer.hpp"

using namespace plog;
using namespace std;
using namespace glm;
using namespace glbinding;
using namespace gl;

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
		/*glbinding::setCallbackMaskExcept(CallbackMask::After |
		                                   CallbackMask::ParametersAndReturnValue,
		                                 {"glGetError"});
		setAfterCallback([](const FunctionCall& call) {
			const GLenum error = glGetError();
			if(error != GL_NO_ERROR) {
				stringstream ss;
				ss << call.function->name() << "(";

				for(unsigned i = 0; i < call.parameters.size(); ++i) {
					ss << call.parameters[i].get();
					if(i < call.parameters.size() - 1) ss << ", ";
				}

				ss << ")";

				if(call.returnValue) { ss << " -> " << call.returnValue.get(); }

				LOG(plog::debug) << ss.str();
				LOG(plog::error) << glbinding::aux::Meta::getString(error);
			}
		});*/
		// Enable v-sync
		glfwSwapInterval(1);

		Renderer renderer(width, height);

		vector<Light> lights;
		Light         light({0, 0, 100.0f},
                {0, 0, 0},
                {255.0f, 255.0f, 255.0f},
                0.01f,
                360.0f,
                1000.0f);
		lights.push_back(light);

		RenderComponent component("ass/Models/Akari/diffuse.dds",
		                          "ass/Models/Akari/diffuse.dds",
		                          "ass/Shaders/textured.vert.glsl",
		                          "ass/Shaders/textured.frag.glsl",
		                          "ass/Models/Akari/model.mdl");

		mat4 model(1.0f);

		mat4 projection =
		  glm::perspective(45.0f,
		                   static_cast<float>(width) / static_cast<float>(height),
		                   0.1f,
		                   10000.0f);

		LOG(debug) << "starting main loop";

		float x = 0.0f;
		while(!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GLFW_TRUE);

			vec3       eye(cos(x) * 70.0f, sin(x) * 70.0f, 30.0f);
			mat4       view = lookAt(eye, {0.0f, 0.0f, 30.0f}, {0.0f, 0.0f, 1.0f});
			RenderTask task(component, 0, model, view, projection, eye, 1.0f, lights);
			renderer.queue(task);
			renderer.draw();
			glfwSwapBuffers(window);
			x += 0.01f;
		}

		LOG(debug) << "loop finished";

		glfwTerminate();
	} catch(const std::exception& exception) {
		LOG(fatal) << exception.what();
		LOG(info) << "exited program unsuccessfully";
		return EXIT_FAILURE;
	}
}
