#include "ActorMotionState.h"

ActorMotionState::ActorMotionState(const btTransform& initialPosition, Actor* actor) : 
m_actor(actor),
m_position(initialPosition) {

}

ActorMotionState::~ActorMotionState() {
	
}

/**
 * Used by Bullet to get the initial position, and subsequent positions for static bodies
 *
 * @param [out] worldTrans  Bullet transformation type to write the current world transformation to
 */
void ActorMotionState::getWorldTransform(btTransform& worldTrans) const {
	worldTrans = m_position;
}

/**
 * Used by Bullet to update the actor's world transformation information based on the results of its simulation
 *
 * @param [in] worldTrans  Bullet transformation type to be parsed and used to update the actor
 */
void ActorMotionState::setWorldTransform(const btTransform& worldTrans) {
	if(m_actor == nullptr) return;
	
	btQuaternion rot = worldTrans.getRotation();
	m_actor->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
	
	btVector3 pos = worldTrans.getOrigin();
	m_actor->setPosition(pos.x(), pos.y(), pos.z());
}
