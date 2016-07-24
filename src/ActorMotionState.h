#ifndef ACTORMOTIONSTATE_H
#define ACTORMOTIONSTATE_H

#include "Actor.h"
#include <btBulletDynamicsCommon.h>

class ActorMotionState : public btMotionState {
	protected:
	Actor*      m_actor;
	btTransform m_position;

	public:
	ActorMotionState(const btTransform& initialPosition, Actor* actor);
	virtual ~ActorMotionState();

	// Renderer -> Bullet
	virtual void getWorldTransform(btTransform& worldTrans) const;

	// Bullet -> Renderer
	virtual void setWorldTransform(const btTransform& worldTrans);
};

#endif
