#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem() {
	m_broadphase = new btDbvtBroadphase();
	
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	
	m_solver = new btSequentialImpulseConstraintSolver();
}

PhysicsSystem::~PhysicsSystem() {
	for(std::pair<Scene*, btDiscreteDynamicsWorld*> pair : m_worlds) delete pair.second;
	
	delete m_broadphase;
	delete m_collisionConfiguration;
	delete m_dispatcher;
	delete m_solver;
}

void PhysicsSystem::simulate(Scene* scene) {
	btDiscreteDynamicsWorld* world;
	if(m_worlds.find(scene) == m_worlds.end()) {
		// Scene has not been registered yet, so we register it
		world = new btDiscreteDynamicsWorld(m_dispatcher, 
	                                      m_broadphase, 
	                                      m_solver, 
	                                      m_collisionConfiguration);
																				
		// TODO: Consult scene for physics constants
		world->setGravity(btVector3(0, -9.8f, 0));
		
		m_worlds.insert(std::pair<Scene*, btDiscreteDynamicsWorld*>(scene, world));
		
		// TODO: Add rigid bodies to the world in the least awful way possible
	}
	else {
		world = m_worlds.at(scene);
	}
	
	// TODO: All systems need to update based on real time
	world->stepSimulation(1/60);
}
