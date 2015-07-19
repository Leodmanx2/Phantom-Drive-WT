#include "DummyActor.h"

DummyActor::DummyActor() {
	try {
		m_renderModel = new RenderModel2D("", "passthrough.vert.glsl", "passthrough.frag.glsl");
	}
	catch(const std::runtime_error& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		g_logger->write(Logger::DEBUG, "Continuing program without initializing DummyActor's render model");
	}
}

DummyActor::~DummyActor() {
	
}
