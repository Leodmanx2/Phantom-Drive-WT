#include "Application.hpp"

// ---------------------------------------------------------------------------
//  Constructors & Destructors
// ---------------------------------------------------------------------------

Application::Application(int argc, char** argv) {
	initFilesystem(argc, argv);
	initGraphics();
	initIO();

	m_renderer = new Renderer(m_window);
	m_scene    = new Scene();
}

Application::~Application() {
	delete m_scene;
	delete m_renderer;
	glfwDestroyWindow(m_window);
	glfwTerminate();
	PHYSFS_deinit();
}

// ---------------------------------------------------------------------------
//  Subsystem initialization helper functions
// ---------------------------------------------------------------------------

void Application::initFilesystem(int, char** argv) {
	g_logger->write(Logger::LOG_INFO, "Initializing virtual file system");
	if(PHYSFS_init(argv[0]) == 0) {
		g_logger->write(Logger::LOG_CRITICAL, PHYSFS_getLastError());
		throw std::runtime_error("Could not initialize virtual file system");
	}

	const std::string assetDir   = PHYSFS_getBaseDir() + std::string("ass");
	const std::string logMessage = "Mounting asset location: " + assetDir;
	g_logger->write(Logger::LOG_INFO, logMessage.c_str());
	if(PHYSFS_mount(assetDir.c_str(), "/", 1) == 0) {
		PHYSFS_deinit();
		g_logger->write(Logger::LOG_CRITICAL, PHYSFS_getLastError());
		throw std::runtime_error("Could not mount asset location");
	}
}

void Application::initGraphics() {
	// Make window and OpenGl context with available extensions
	glfwSetErrorCallback(error_callback);
	if(!glfwInit()) { throw std::runtime_error("GLFW initialization failed"); }
#ifdef DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_window = glfwCreateWindow(
	  default_width, default_height, "Hello World!", nullptr, nullptr);
	if(!m_window) {
		PHYSFS_deinit();
		glfwTerminate();
		throw std::runtime_error("Window or OpenGL context could not be created");
	}
	glfwMakeContextCurrent(m_window);
	glbinding::Binding::initialize();

// Enable automatic logging of OpenGL calls in debug mode
#ifdef DEBUG
	glbinding::setCallbackMaskExcept(glbinding::CallbackMask::After,
	                                 {"glGetError"});
	glbinding::setAfterCallback([](const glbinding::FunctionCall& call) {
		const auto error = gl::glGetError();
		if(error != gl::GL_NO_ERROR) {
			std::stringstream ss("OpenGL Error: ");
			ss << std::hex << error << " after function: " << call.function->name();
			g_logger->write(Logger::LOG_DEBUG, ss.str());
		}
	});
#endif

	// Log loaded OpenGl version
	std::stringstream version;
	version << "OpenGl context version: " << gl::glGetString(gl::GL_VERSION);
	g_logger->write(Logger::LOG_INFO, version.str());

	// Enable v-sync
	glfwSwapInterval(1);

	// Enable back-face culling, z-buffering, and anti-aliasing
	gl::glEnable(gl::GL_CULL_FACE);
	gl::glEnable(gl::GL_DEPTH_TEST);
	gl::glEnable(gl::GL_LINE_SMOOTH);
}

void Application::initIO() {
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, 1);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// ---------------------------------------------------------------------------
//  Internal utility functions
// ---------------------------------------------------------------------------

void Application::error_callback(int, const char* description) {
	g_logger->write(Logger::LOG_ERROR, description);
}

void Application::processInput() {
	glfwPollEvents();

	if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);

	m_scene->processInput(*m_window);
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
	while(!glfwWindowShouldClose(m_window)) {
		std::chrono::milliseconds timeStep =
		  std::chrono::duration_cast<std::chrono::milliseconds>(newTime - oldTime);
		m_scene->update(timeStep);
		m_renderer->draw(*m_scene);
		processInput();
		oldTime = newTime;
		newTime = std::chrono::steady_clock::now();
	}
}
