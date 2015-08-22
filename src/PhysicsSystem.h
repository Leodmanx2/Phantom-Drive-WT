#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <btBulletDynamicsCommon.h>

class PhysicsSystem {
	private:
		btBroadphaseInterface* m_broadphase;
    btDefaultCollisionConfiguration* m_collisionConfiguration;
    btCollisionDispatcher* m_dispatcher;
		btSequentialImpulseConstraintSolver* m_solver;
    btDiscreteDynamicsWorld* m_dynamicsWorld;
	
	public:
		PhysicsSystem();
		~PhysicsSystem();
};

#endif
