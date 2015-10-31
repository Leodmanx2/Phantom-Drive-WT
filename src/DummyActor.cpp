#include "DummyActor.h"

std::unique_ptr<btCollisionShape> DummyActor::s_collisionShape(new btBoxShape(btVector3(1.0f, 1.0f, 1.0f)));

DummyActor::DummyActor(PhysicsSimulator* simulator) : SimulatedPhysicsActor(simulator, 1.0f) {
	try {
		m_renderModel = new RenderModel2D("test.dds", "passthrough.vert.glsl", "passthrough.frag.glsl");
	}
	catch(const std::runtime_error& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		g_logger->write(Logger::DEBUG, "Continuing program without initializing DummyActor's render model");
	}
}

DummyActor::~DummyActor() {
	
}
