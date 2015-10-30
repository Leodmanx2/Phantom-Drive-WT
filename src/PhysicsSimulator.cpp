#include "PhysicsSimulator.h"

PhysicsSimulator::PhysicsSimulator() {
	m_broadphase = new btDbvtBroadphase();
	
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	
	m_solver = new btSequentialImpulseConstraintSolver();
	
	m_world = new btDiscreteDynamicsWorld(m_dispatcher, 
	                                      m_broadphase, 
	                                      m_solver, 
	                                      m_collisionConfiguration);
	
	m_world->setGravity(btVector3(0.0f, -0.98f, 0.0f));
}

PhysicsSimulator::~PhysicsSimulator() {
	delete m_broadphase;
	delete m_collisionConfiguration;
	delete m_dispatcher;
	delete m_solver;
	delete m_world;
}

void PhysicsSimulator::addRigidBody(btRigidBody* body) {
	m_world->addRigidBody(body);
}

void PhysicsSimulator::removeRigidBody(btRigidBody* body) {
	m_world->removeRigidBody(body);
}

void PhysicsSimulator::stepSimulation() {
	// TODO: All systems need to update based on real time
	m_world->stepSimulation(1/60.0f, 10);
}
