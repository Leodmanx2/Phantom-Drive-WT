#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <btBulletDynamicsCommon.h>
#include <map>
#include "Scene.h"

class PhysicsSystem {
	private:
		btBroadphaseInterface* m_broadphase;
    btDefaultCollisionConfiguration* m_collisionConfiguration;
    btCollisionDispatcher* m_dispatcher;
		btSequentialImpulseConstraintSolver* m_solver;
		
		// We keep a collection of separate worlds for each scene to
		// spare the time and uncertainty of adding and removing bodies
		// each frame
    std::map<Scene*, btDiscreteDynamicsWorld*> m_worlds;
	
	public:
		PhysicsSystem();
		~PhysicsSystem();
		
		void simulate(Scene* scene);
};

#endif
