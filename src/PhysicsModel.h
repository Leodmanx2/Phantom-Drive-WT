#ifndef PHYSICSMODEL_H
#define PHYSICSMODEL_H

#include <btBulletDynamicsCommon.h>

class PhysicsModel {
	private:
		btCollisionShape* m_collisionShape;
		btMotionState* m_motionState;
		btRigidBody* m_body;
	
	public:
		PhysicsModel::PhysicsModel(btScalar mass, btCollisionShape* collisionShape)
		~PhysicsModel();
		
		const btRigidBody* getBody() const;
}

#endif
