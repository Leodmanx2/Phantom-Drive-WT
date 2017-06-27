#include "Application.hpp"

std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>>
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
	g_logger.write(Logger::LogLevel::LOG_INFO,
	               "Initializing virtual file system");
	if(PHYSFS_init(argv[0]) == 0) {
		g_logger.write(Logger::LogLevel::LOG_CRITICAL, PHYSFS_getLastError());
		throw std::runtime_error("Could not initialize virtual file system");
	}

	const std::string assetDir   = PHYSFS_getBaseDir() + std::string("ass");
	const std::string logMessage = "Mounting asset location: " + assetDir;
	g_logger.write(Logger::LogLevel::LOG_INFO, logMessage.c_str());
	if(PHYSFS_mount(assetDir.c_str(), "/", 1) == 0) {
		PHYSFS_deinit();
		g_logger.write(Logger::LogLevel::LOG_CRITICAL, PHYSFS_getLastError());
		throw std::runtime_error("Could not mount asset location");
	}
}

// ---------------------------------------------------------------------------
//  Internal utility functions
// ---------------------------------------------------------------------------

void Application::key_callback(
  GLFWwindow*, int key, int, int action, int modifiers) {
	// System repeat events are not useful for our purposes. They do not occur
	// every frame and may not occur if other keys are pressed. Scenes must
	// simulate their own repeat events.
	if(action == GLFW_REPEAT) return;

	if(action == GLFW_PRESS) { s_keysPressed.emplace(key, modifiers); };
	if(action == GLFW_RELEASE) { s_keysPressed.erase({key, modifiers}); };

	std::lock_guard<std::mutex> lock(s_keyQueueMutex);
	s_keyQueue.emplace<KeyEvent>({key, action, modifiers});
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
		// Generate a REPEAT event for all keys that have been pressed but have not
		// been released
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
