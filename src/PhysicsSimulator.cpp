#include "PhysicsSimulator.h"

std::unique_ptr<btBroadphaseInterface> PhysicsSimulator::s_broadphase(new btDbvtBroadphase());
std::unique_ptr<btDefaultCollisionConfiguration> PhysicsSimulator::s_collisionConfiguration(new btDefaultCollisionConfiguration());
std::unique_ptr<btCollisionDispatcher> PhysicsSimulator::s_dispatcher(new btCollisionDispatcher(s_collisionConfiguration.get()));
std::unique_ptr<btSequentialImpulseConstraintSolver> PhysicsSimulator::s_solver(new btSequentialImpulseConstraintSolver());

PhysicsSimulator::PhysicsSimulator() {
	m_world = new btDiscreteDynamicsWorld(s_dispatcher.get(), 
	                                      s_broadphase.get(), 
	                                      s_solver.get(), 
	                                      s_collisionConfiguration.get());
	
	m_world->setGravity(btVector3(0.0f, -0.98f, 0.0f));
}

PhysicsSimulator::PhysicsSimulator(PhysicsSimulator& original) {
	m_world = new btDiscreteDynamicsWorld(*original.m_world);
}

PhysicsSimulator::~PhysicsSimulator() {
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
