#ifndef PHYSICSMODEL_H
#define PHYSICSMODEL_H

#include <btBulletDynamicsCommon.h>

class PhysicsModel {
	private:
		btMotionState* m_motionState;
		btRigidBody* m_body;
	
	public:
		PhysicsModel(btScalar mass, btCollisionShape* collisionShape);
		~PhysicsModel();
		
		const btRigidBody* getBody() const;
};

#endif
