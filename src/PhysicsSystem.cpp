#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem() {
	m_broadphase = new btDbvtBroadphase();
	
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	
	m_solver = new btSequentialImpulseConstraintSolver();

	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, 
	                                              m_broadphase, 
	                                              m_solver, 
	                                              m_collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));
}

PhysicsSystem::~PhysicsSystem() {
	delete m_broadphase;
	delete m_collisionConfiguration;
	delete m_dispatcher;
	delete m_solver;
	delete m_dynamicsWorld;
}
