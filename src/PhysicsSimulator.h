#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <btBulletDynamicsCommon.h>

class PhysicsSimulator {
	private:
		btBroadphaseInterface* m_broadphase;
    btDefaultCollisionConfiguration* m_collisionConfiguration;
    btCollisionDispatcher* m_dispatcher;
		btSequentialImpulseConstraintSolver* m_solver;
		btDiscreteDynamicsWorld* m_world;
	
	public:
		PhysicsSimulator();
		~PhysicsSimulator();
		
		void addRigidBody(btRigidBody* body);
		void removeRigidBody(btRigidBody* body);
		void stepSimulation();
};

#endif
