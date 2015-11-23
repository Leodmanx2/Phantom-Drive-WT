#include "DummyActor2.h"

DummyActor2::DummyActor2() {
	try {
		g_logger->write(Logger::DEBUG, "Making render model");
		m_renderModel = new RenderModel3D("testModel.obj", "passthrough.vert.glsl", "passthrough.frag.glsl");
	}
	catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		g_logger->write(Logger::DEBUG, "Continuing program without initializing DummyActor's render model");
	}
}

DummyActor2::~DummyActor2() {
	
}
