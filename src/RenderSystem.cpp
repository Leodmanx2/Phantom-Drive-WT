#include "RenderSystem.h"

RenderSystem::RenderSystem() {
	int width = 640;
	int height = 480;
	
	try { m_window = new Window(width, height); }
	catch(const std::runtime_error& exception) {
		g_logger->write(Logger::CRITICAL, exception.what());
		throw std::runtime_error("Failed to create window");
	}
	
	m_context = SDL_GL_CreateContext(m_window->SDL_Pointer());
	if(m_context == nullptr) {
		delete m_window;
		g_logger->write(Logger::CRITICAL, SDL_GetError());
		throw std::runtime_error("Could not create OpenGL context");
	}
	
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		g_logger->write(Logger::CRITICAL, (const char*)glewGetErrorString(err));
		throw std::runtime_error("Failed to initialize GLEW");
	}
	
	m_projectionMatrix = glm::ortho(0.0f, (float)width, (float)height, 0.0f, 0.1f, 100.0f);
	
	m_camera = new Camera();

	m_actor = new DummyActor();
}

RenderSystem::RenderSystem(RenderSystem& original) {
		m_window = new Window(*original.m_window);
		m_camera = new Camera(*original.m_camera);
		m_actor = new Actor(*original.m_actor);
}

RenderSystem::~RenderSystem() {
	SDL_GL_DeleteContext(m_context);
	delete m_window;
	delete m_actor;
}

void RenderSystem::draw() {
	glClearColor( 0.53f, 0.88f, 0.96f, 0.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	const float* projectionDataOrig = glm::value_ptr(m_projectionMatrix);
	float projectionData[16];
	for(int i=0; i<16; ++i) {
		projectionData[i] = projectionDataOrig[i];
	}
	
	m_actor->draw(m_camera->getViewMatrix(), projectionData);
	
	SDL_GL_SwapWindow(m_window->SDL_Pointer());
}

void RenderSystem::resizeWindow(unsigned int width, unsigned int height) {
	m_projectionMatrix = glm::ortho(0.0f, (float)width, (float)height, 0.0f, 0.1f, 100.0f);
	m_window->resize(width, height);
}
