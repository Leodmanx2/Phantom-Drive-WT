#include "ActorMotionState.h"

ActorMotionState::ActorMotionState(const btTransform& initialPosition, Actor* actor) {
	m_actor = actor;
	m_position = initialPosition;
}

ActorMotionState::~ActorMotionState() {
	
}

void ActorMotionState::setActor(Actor* actor) {
	m_actor = actor;
}

void ActorMotionState::getWorldTransform(btTransform& worldTrans) const {
	worldTrans = m_position;
}

void ActorMotionState::setWorldTransform(const btTransform& worldTrans) {
	if(m_actor == nullptr) return;
	
	btQuaternion rot = worldTrans.getRotation();
	m_actor->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
	
	btVector3 pos = worldTrans.getOrigin();
	m_actor->setPosition(pos.x(), pos.y(), pos.z());
}
