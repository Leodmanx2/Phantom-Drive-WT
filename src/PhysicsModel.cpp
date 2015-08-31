#include "PhysicsModel.h"

// TODO: Class is tied to Bullet. Change that.
//       Public elements should not be bound to the implementation.

PhysicsModel::PhysicsModel(btScalar mass, btCollisionShape* collisionShape) : 
m_collisionShape(collisionShape) {
	// TODO: Collision shapes should be shared among identically shaped Actors
	btVector3 inertia(0, 0, 0);
	
	m_collisionShape->calculateLocalInertia(mass, inertia);
	
	m_motionState = new btDefaultMotionState();
	
  btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, 
	                                                          m_motionState, 
	                                                          m_collisionShape, 
	                                                          inertia);
	
  m_body = new btRigidBody(constructionInfo);
}

PhysicsModel::~PhysicsModel() {
	delete m_collisionShape;
	delete m_motionState;
	delete m_body;
}

const btRigidBody* PhysicsModel::getBody() const {
	return m_body;
}
