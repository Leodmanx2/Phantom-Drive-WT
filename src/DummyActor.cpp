#include "DummyActor.h"

DummyActor::DummyActor() {
	try {
		g_logger->write(Logger::DEBUG, "Making render model");
		m_renderModel = new RenderModel("TestCube");
	}
	catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		g_logger->write(Logger::DEBUG, "Continuing program without initializing DummyActor's render model");
	}
}
