#include "SimulatedPhysicsActor.h"

std::unique_ptr<btCollisionShape> SimulatedPhysicsActor::s_defaultCollisionShape(new btBoxShape(btVector3(1.0f, 1.0f, 1.0f)));

SimulatedPhysicsActor::SimulatedPhysicsActor(PhysicsSimulator* simulator, btScalar mass) {
	// Physics setup
	btVector3 inertia(0.0f, 0.0f, 0.0f);
	s_defaultCollisionShape->calculateLocalInertia(mass, inertia);
	
	btQuaternion initialOrientation(m_orientation.x, m_orientation.y, m_orientation.z, m_orientation.w);
	btVector3 initialPosition(0.0f, 0.0f, 0.0f);
	btTransform initialTrans(initialOrientation, initialPosition);
	btMotionState* motionState = new ActorMotionState(initialTrans, this);
	
  btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, 
	                                                          motionState, 
	                                                          s_defaultCollisionShape.get(), 
	                                                          inertia);
  m_body = new btRigidBody(constructionInfo);
	
	simulator->addRigidBody(m_body);
	
	// We keep the pointer so that we can remove the body from the simulation on
	// destruction
	m_simulator = simulator;
}

SimulatedPhysicsActor::~SimulatedPhysicsActor() {
	m_simulator->removeRigidBody(m_body);
	delete m_body;
}

// TODO: document functions
void SimulatedPhysicsActor::translate(float longitude, float latitude, float altitude) {
	btVector3 originalPosition(m_position.x, m_position.y, m_position.z);
	
	Actor::translate(longitude, latitude, altitude);
	
	btVector3 newPosition(m_position.x, m_position.y, m_position.z);
	
	btTransform originalTransform = m_body->getWorldTransform();
	
	btTransform diffTransform;
	diffTransform.setIdentity();
	diffTransform.setOrigin(newPosition - originalPosition);
	
	btTransform newTransform = originalTransform * diffTransform;
	
	m_body->setWorldTransform(newTransform);
}

void SimulatedPhysicsActor::rotate(float roll, float pitch, float yaw) {
	btQuaternion originalOrientation(m_orientation.x, m_orientation.y, m_orientation.z, m_orientation.w);
	
	Actor::rotate(roll, pitch, yaw);
	
	btQuaternion newOrientation(m_orientation.x, m_orientation.y, m_orientation.z, m_orientation.w);
	
	btTransform originalTransform = m_body->getWorldTransform();
	
	btTransform diffTransform;
	diffTransform.setIdentity();
	diffTransform.setRotation(newOrientation - originalOrientation);
	
	btTransform newTransform = originalTransform * diffTransform;
	
	m_body->setWorldTransform(newTransform);
}
