#include "Application.hpp"

#define GLFW_INCLUDE_NONE

#include "RenderContext.hpp"
#include "Renderer.hpp"
#include "ShaderProgram.hpp"
#include "SpatialComponent.hpp"

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

const int INIT_WIDTH  = 640;
const int INIT_HEIGHT = 480;

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
	auto frame_buffer = make_unique<PD::Framebuffer>(INIT_WIDTH, INIT_HEIGHT);

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

	// Load model assets
	Geometry         geometry("model.mdl");
	auto             albedo = PD::load_texture("albedo.dds");
	const int        id     = 0;
	SpatialComponent spatial;

	// Define scene parameters
	const double ambience = 1.0;

	// Define lights
	std::vector<Light> lights;
	Light              light(
    {10.0f, 0, 0.0f}, {0, 0, 0}, {255.0f, 0.0f, 0.0f}, 0.05f, 360.0f, 1000.0f);
	Light light2(
	  {-10.0f, 0, 0.0f}, {0, 0, 0}, {0.0f, 0.0f, 255.0f}, 0.05f, 360.0f, 1000.0f);
	lights.push_back(light);
	lights.push_back(light2);

	// Define view transform
	glm::vec3 eye(0.0, 0.0, -10.0);
	glm::vec3 up(0.0, 1.0, 0.0);
	glm::vec3 center(spatial.position());
	auto      view = glm::lookAt(eye, center, up);

	// Define projection transform
	const double     field_of_view = 45.0;
	constexpr double aspect_ratio =
	  static_cast<double>(INIT_WIDTH) / static_cast<double>(INIT_HEIGHT);
	const double near_plane_distance = 0.1;
	const double far_plane_distance  = 10'000;
	auto         projection          = glm::perspective(
    field_of_view, aspect_ratio, near_plane_distance, far_plane_distance);

	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		context->draw({albedo.get()},
		              geometry,
		              id,
		              {spatial.matrix(), view, projection},
		              eye,
		              ambience,
		              lights.begin(),
		              lights.end());
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
}
