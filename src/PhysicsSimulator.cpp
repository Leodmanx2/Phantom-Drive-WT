#include "PhysicsSimulator.hpp"

std::unique_ptr<btBroadphaseInterface>
  PhysicsSimulator::s_broadphase(new btDbvtBroadphase());
std::unique_ptr<btDefaultCollisionConfiguration>
  PhysicsSimulator::s_collisionConfiguration(
    new btDefaultCollisionConfiguration());
std::unique_ptr<btCollisionDispatcher> PhysicsSimulator::s_dispatcher(
  new btCollisionDispatcher(s_collisionConfiguration.get()));
std::unique_ptr<btSequentialImpulseConstraintSolver>
  PhysicsSimulator::s_solver(new btSequentialImpulseConstraintSolver());

PhysicsSimulator::PhysicsSimulator() {
	m_world = new btDiscreteDynamicsWorld(s_dispatcher.get(),
	                                      s_broadphase.get(),
	                                      s_solver.get(),
	                                      s_collisionConfiguration.get());

	m_world->setGravity(btVector3(0.0f, -0.98f, 0.0f));
}

PhysicsSimulator::PhysicsSimulator(PhysicsSimulator& original)
  : m_world(new btDiscreteDynamicsWorld(*original.m_world)) {}

PhysicsSimulator& PhysicsSimulator::
operator=(const PhysicsSimulator& original) {
	if(this != &original) {
		m_world = new btDiscreteDynamicsWorld(*original.m_world);
	}
	return *this;
}

PhysicsSimulator::~PhysicsSimulator() { delete m_world; }

/**
 * Adds a Bullet rigid body to be simulated
 *
 * @param [in] body  The body to be simulated
 */
void PhysicsSimulator::addRigidBody(btRigidBody* body) {
	m_world->addRigidBody(body);
}

/**
 * Removes a body from the simulation
 *
 * @param [in] body  The body to be removed
 */
void PhysicsSimulator::removeRigidBody(btRigidBody* body) {
	m_world->removeRigidBody(body);
}

/**
 * Called once per cycle in order to advance the simulation
 *
 * @param [in] duration   Time in milliseconds since the last simulation step
 */
void PhysicsSimulator::stepSimulation(std::chrono::milliseconds duration) {
	btScalar timeStep = duration.count() / 1000.0f;
	m_world->stepSimulation(timeStep);
}
