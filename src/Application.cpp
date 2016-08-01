#include "Application.hpp"

Application::Application(int argc, char** argv) {
	g_logger->write(Logger::LOG_INFO, "Initializing virtual file system");
	if(PHYSFS_init(argv[0]) == 0) {
		g_logger->write(Logger::LOG_CRITICAL, PHYSFS_getLastError());
		throw std::runtime_error("Could not initialize virtual file system");
	}

	std::string assetDir   = PHYSFS_getBaseDir() + std::string("ass");
	std::string logMessage = "Mounting asset location: " + assetDir;
	g_logger->write(Logger::LOG_INFO, logMessage.c_str());
	if(PHYSFS_mount(assetDir.c_str(), "/", 1) == 0) {
		g_logger->write(Logger::LOG_CRITICAL, PHYSFS_getLastError());
		throw std::runtime_error("Could not mount asset files");
	}

	glfwSetErrorCallback(error_callback);
	if(!glfwInit()) { throw std::runtime_error("GLFW initialization failed"); }

	try {
		renderInit();
	} catch(const std::exception& exception) {
		glfwTerminate();
		g_logger->write(Logger::LOG_CRITICAL, exception.what());
		throw std::runtime_error("Could not initialize rendering system");
	}

	m_scene = new Scene();
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, 1);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Application::~Application() {
	delete m_scene;
	glfwDestroyWindow(m_window);
	glfwTerminate();
	PHYSFS_deinit();
}

void Application::error_callback(int error, const char* description) {
	g_logger->write(Logger::LOG_ERROR, description);
}

/**
 * Needs to be called once in order for the application to begin processing
 */
void Application::run() {
	while(!glfwWindowShouldClose(m_window)) {
		m_scene->simulate();
		m_scene->update();
		draw(*m_scene);
		processInput();
	}
}

void Application::renderInit() {
	int width  = 640;
	int height = 480;

#ifdef DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_window = glfwCreateWindow(width, height, "Hello World!", nullptr, nullptr);
	if(!m_window) {
		throw std::runtime_error("Window or OpenGL context could not be created");
	}

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	glbinding::Binding::initialize();

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

	m_projectionMatrix =
	  glm::perspective(45.0f,
	                   static_cast<float>(width) / static_cast<float>(height),
	                   0.1f,
	                   10000.0f);

	glInit();
}

void Application::glInit() {
	gl::glEnable(gl::GL_CULL_FACE);
	gl::glEnable(gl::GL_DEPTH_TEST);
	gl::glEnable(gl::GL_LINE_SMOOTH);
}

/** Requests that a scene draw itself onto the system's active rendering buffer
 *
 * @param [in] scene  The scene to be drawn
 */
void Application::draw(Scene& scene) {
	//glClearColor( 0.53f, 0.88f, 0.96f, 0.0f );
	gl::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

	scene.draw(m_projectionMatrix);

	glfwSwapBuffers(m_window);
}

/**
 * Resizes the window so that the viewport matches the input parameters
 *
 * @param [in] width   The width of the viewport to size the window to
 * @param [in] height  The height of the viewport to size the widnow to
 */
void Application::resizeWindow(unsigned int width, unsigned int height) {
	m_projectionMatrix = glm::ortho(0.0f,
	                                static_cast<float>(width),
	                                static_cast<float>(height),
	                                0.0f,
	                                0.1f,
	                                100000.0f);

	glfwSetWindowSize(m_window, width, height);
}

void Application::processInput() {
	glfwPollEvents();

	if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);

	m_scene->processInput(*m_window);
}
