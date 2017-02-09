#include "Application.hpp"

// ---------------------------------------------------------------------------
//  Constructors & Destructors
// ---------------------------------------------------------------------------

Application::Application(int argc, char** argv) {
	initFilesystem(argc, argv);
	initGraphics();
	initIO();

	m_scene = new Scene();
}

Application::~Application() {
	delete m_scene;
	gl::glDeleteFramebuffers(1, &m_frameBuffer);
	gl::glDeleteRenderbuffers(1, &colorAttachment);
	gl::glDeleteRenderbuffers(1, &selectionAttachment);
	gl::glDeleteRenderbuffers(1, &depthStencilAttachment);
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

	// Build custom framebuffer
	// TODO: Delete these structures when the application stops
	gl::glGenFramebuffers(1, &m_frameBuffer);
	gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, m_frameBuffer);

	gl::glGenRenderbuffers(1, &colorAttachment);
	gl::glBindRenderbuffer(gl::GL_RENDERBUFFER, colorAttachment);
	gl::glRenderbufferStorage(
	  gl::GL_RENDERBUFFER, gl::GL_RGBA32F, default_width, default_height);
	gl::glFramebufferRenderbuffer(gl::GL_FRAMEBUFFER,
	                              gl::GL_COLOR_ATTACHMENT0,
	                              gl::GL_RENDERBUFFER,
	                              colorAttachment);

	gl::glGenRenderbuffers(1, &selectionAttachment);
	gl::glBindRenderbuffer(gl::GL_RENDERBUFFER, selectionAttachment);
	gl::glRenderbufferStorage(
	  gl::GL_RENDERBUFFER, gl::GL_R32UI, default_width, default_height);
	gl::glFramebufferRenderbuffer(gl::GL_FRAMEBUFFER,
	                              gl::GL_COLOR_ATTACHMENT1,
	                              gl::GL_RENDERBUFFER,
	                              selectionAttachment);

	gl::glGenRenderbuffers(1, &depthStencilAttachment);
	gl::glBindRenderbuffer(gl::GL_RENDERBUFFER, depthStencilAttachment);
	gl::glRenderbufferStorage(gl::GL_RENDERBUFFER,
	                          gl::GL_DEPTH24_STENCIL8,
	                          default_width,
	                          default_height);
	gl::glFramebufferRenderbuffer(gl::GL_FRAMEBUFFER,
	                              gl::GL_DEPTH_STENCIL_ATTACHMENT,
	                              gl::GL_RENDERBUFFER,
	                              depthStencilAttachment);

	gl::GLenum stat = gl::glCheckFramebufferStatus(gl::GL_FRAMEBUFFER);
	if(stat != gl::GL_FRAMEBUFFER_COMPLETE) {
		PHYSFS_deinit();
		glfwTerminate();
		gl::glDeleteFramebuffers(1, &m_frameBuffer);
		gl::glDeleteRenderbuffers(1, &colorAttachment);
		gl::glDeleteRenderbuffers(1, &selectionAttachment);
		gl::glDeleteRenderbuffers(1, &depthStencilAttachment);
		throw std::runtime_error("Could not build framebuffer");
	}

	// Enable v-sync
	glfwSwapInterval(1);

	// Enable back-face culling, z-buffering, and anti-aliasing
	gl::glEnable(gl::GL_CULL_FACE);
	gl::glEnable(gl::GL_DEPTH_TEST);
	gl::glEnable(gl::GL_LINE_SMOOTH);

	m_projectionMatrix = glm::perspective(45.0f,
	                                      static_cast<float>(default_width) /
	                                        static_cast<float>(default_height),
	                                      0.1f,
	                                      10000.0f);
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
		draw(*m_scene);
		processInput();
		oldTime = newTime;
		newTime = std::chrono::steady_clock::now();
	}
}

/** Requests that a scene draw itself onto the system's active rendering buffer
 *
 * @param [in] scene  The scene to be drawn
 */
void Application::draw(Scene& scene) {
	gl::glClearColor(0.53f, 0.88f, 0.96f, 0.0f);
	//gl::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

	gl::glBindFramebuffer(gl::GL_DRAW_FRAMEBUFFER, m_frameBuffer);
	gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
	gl::GLenum drawBuffers[2] = {gl::GL_COLOR_ATTACHMENT0,
	                             gl::GL_COLOR_ATTACHMENT1};
	gl::glDrawBuffers(2, drawBuffers);
	scene.draw(m_projectionMatrix);

	gl::glBindFramebuffer(gl::GL_READ_FRAMEBUFFER, m_frameBuffer);
	gl::glBindFramebuffer(gl::GL_DRAW_FRAMEBUFFER, 0);
	// TODO: Size framebuffer to window
	gl::glBlitFramebuffer(0,
	                      0,
	                      default_width,
	                      default_height,
	                      0,
	                      0,
	                      default_width,
	                      default_height,
	                      gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT |
	                        gl::GL_STENCIL_BUFFER_BIT,
	                      gl::GL_NEAREST);

	gl::glBindFramebuffer(gl::GL_READ_FRAMEBUFFER, 0);

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
