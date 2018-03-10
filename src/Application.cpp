#include "Application.hpp"

#include "Actor.hpp"
#include "EditorScene.hpp"
#include "Window.hpp"
#include <plog/Log.h>

using namespace plog;

std::unordered_set<std::pair<int, int>, Application::KeyHash>
  Application::s_keysPressed;

std::mutex           Application::s_keyQueueMutex;
std::queue<KeyEvent> Application::s_keyQueue;

std::mutex                   Application::s_buttonQueueMutex;
std::queue<MouseButtonEvent> Application::s_buttonQueue;

std::mutex                     Application::s_movementQueueMutex;
std::queue<MouseMovementEvent> Application::s_movementQueue;

// ---------------------------------------------------------------------------
//  Constructors & Destructors
// ---------------------------------------------------------------------------

Application::Application(int argc, char** argv)
  : m_window(new Window()), m_renderer(m_window) {
	initFilesystem(argc, argv);

	glfwSetKeyCallback(&m_window->get(), key_callback);
	glfwSetMouseButtonCallback(&m_window->get(), mouse_button_callback);
	glfwSetCursorPosCallback(&m_window->get(), cursor_position_callback);

	m_scene = new EditorScene("DevScene");
}

Application::~Application() {
	delete m_scene;
	PHYSFS_deinit();
}

// ---------------------------------------------------------------------------
//  Subsystem initialization helper functions
// ---------------------------------------------------------------------------

void Application::initFilesystem(int, char** argv) {
	LOG(info) << "Initializing virtual file system";
	if(PHYSFS_init(argv[0]) == 0) {
		LOG(fatal) << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
		throw std::runtime_error("could not initialize virtual file system");
	}

	const std::string assetDir = PHYSFS_getBaseDir() + std::string("ass");
	LOG(info) << "mounting asset location: " << assetDir;
	if(PHYSFS_mount(assetDir.c_str(), "/", 1) == 0) {
		PHYSFS_deinit();
		LOG(fatal) << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
		throw std::runtime_error("could not mount asset location");
	}
}

// ---------------------------------------------------------------------------
//  Internal utility functions
// ---------------------------------------------------------------------------

void Application::key_callback(
  GLFWwindow*, int key, int, int action, int modifiers) {
	// System repeat events are not useful for our purposes. They do not occur
	// every frame and may not occur if other keys are pressed. We simulate our
	// own repeat events on each frame instead.
	if(action == GLFW_REPEAT) return;

	// The placement of this expression ensures no REPEAT event occurs after a
	// RELEASED event in the queue
	if(action == GLFW_RELEASE) { s_keysPressed.erase({key, modifiers}); };

	// Add the event to the queue
	std::lock_guard<std::mutex> lock(s_keyQueueMutex);
	s_keyQueue.emplace<KeyEvent>({key, action, modifiers});

	// The placement of this expression ensures all REPEAT events occur after
	// PRESSED events in the queue
	if(action == GLFW_PRESS) { s_keysPressed.emplace(key, modifiers); };
}

void Application::mouse_button_callback(GLFWwindow*,
                                        int button,
                                        int action,
                                        int modifiers) {
	std::lock_guard<std::mutex> lock(s_buttonQueueMutex);
	s_buttonQueue.emplace<MouseButtonEvent>({button, action, modifiers});
}

void Application::cursor_position_callback(GLFWwindow*,
                                           double xpos,
                                           double ypos) {
	static double x0 = xpos;
	static double y0 = ypos;
	{
		std::lock_guard<std::mutex> lock(s_movementQueueMutex);
		s_movementQueue.emplace<MouseMovementEvent>({xpos - x0, ypos - y0});
	}
	x0 = xpos;
	y0 = ypos;
}

void Application::processInput() {
	glfwPollEvents();
	GLFWwindow& window = m_window->get();
	if(glfwGetKey(&window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(&window, GLFW_TRUE);

	{
		std::lock_guard<std::mutex> lock(s_keyQueueMutex);
		// Make a REPEAT event for all keys that have been pressed but not released
		// REASONING: Input bindings are only defined at runtime, so we cannot use
		//            glfwGetKey() without polling every key each frame.
		for(auto key : s_keysPressed) {
			s_keyQueue.emplace<KeyEvent>({key.first, GLFW_REPEAT, key.second});
		}
		m_scene->process(s_keyQueue);
	}

	{
		std::lock_guard<std::mutex> lock(s_buttonQueueMutex);
		m_scene->process(s_buttonQueue);
	}

	{
		std::lock_guard<std::mutex> lock(s_movementQueueMutex);
		m_scene->process(s_movementQueue);
	}
}

void Application::draw() {
	m_renderer.clear();
	m_renderer.startNormalPass();
	m_scene->draw();
	m_renderer.finishNormalPass();
	glfwSwapBuffers(&m_window->get());
}

// ---------------------------------------------------------------------------
//  Public-facing functions
// ---------------------------------------------------------------------------

/**
 * Needs to be called once in order for the application to begin processing
 */
void Application::run() {
	std::chrono::time_point<std::chrono::steady_clock> oldTime, newTime;
	oldTime = newTime = std::chrono::steady_clock::now();
	while(!glfwWindowShouldClose(&m_window->get())) {
		std::chrono::milliseconds timeStep =
		  std::chrono::duration_cast<std::chrono::milliseconds>(newTime - oldTime);
		m_scene->update(timeStep);
		draw();
		processInput();
		oldTime = newTime;
		newTime = std::chrono::steady_clock::now();
	}
}
