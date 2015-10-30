#include "DummyActor.h"

std::unique_ptr<btCollisionShape> DummyActor::s_collisionShape(new btBoxShape(btVector3(1.0f, 1.0f, 1.0f)));

DummyActor::DummyActor(PhysicsSimulator* simulator) {
	try {
		m_renderModel = new RenderModel2D("test.dds", "passthrough.vert.glsl", "passthrough.frag.glsl");
	}
	catch(const std::runtime_error& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		g_logger->write(Logger::DEBUG, "Continuing program without initializing DummyActor's render model");
	}
	
	// Physics setup
	btVector3 inertia(0.0f, 0.0f, 0.0f);
	btScalar mass = 1.0f;
	s_collisionShape->calculateLocalInertia(mass, inertia);
	
	btTransform initialTrans(btQuaternion(45.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, 0.0f, 0.0f));
	btMotionState* motionState = new ActorMotionState(initialTrans, this);
	
  btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, 
	                                                          motionState, 
	                                                          s_collisionShape.get(), 
	                                                          inertia);
  m_body = new btRigidBody(constructionInfo);
	
	simulator->addRigidBody(m_body);
	
	// We keep the pointer so that we can remove the body from the simulation on
	// destruction
	m_simulator = simulator;
}

DummyActor::~DummyActor() {
	m_simulator->removeRigidBody(m_body);
}

void DummyActor::update() {
	
}
