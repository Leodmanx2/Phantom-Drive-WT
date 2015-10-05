#include "PhysicsModel.h"

// TODO: Public interface is tied to Bullet. Change that. Look to remove Bullet code from RenderModel* as well.

PhysicsModel::PhysicsModel(btScalar mass, btCollisionShape* collisionShape) {
	btVector3 inertia(0, 0, 0);
	
	collisionShape->calculateLocalInertia(mass, inertia);
	
	m_motionState = new btDefaultMotionState();
	
  btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, 
	                                                          m_motionState, 
	                                                          collisionShape, 
	                                                          inertia);
	
  m_body = new btRigidBody(constructionInfo);
}

PhysicsModel::~PhysicsModel() {
	delete m_motionState;
	delete m_body;
}

const btRigidBody* PhysicsModel::getBody() const {
	return m_body;
}
