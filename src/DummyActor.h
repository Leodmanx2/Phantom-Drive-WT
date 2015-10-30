#ifndef DUMMYACTOR_H
#define DUMMYACTOR_H

#include "Actor.h"
#include "RenderModel2D.h"
#include <GL/glew.h>
#include <memory>
#include "Logger.h"
#include <stdexcept>
#include "PhysicsSimulator.h"
#include "ActorMotionState.h"

class DummyActor : public Actor {
	private:
		static std::unique_ptr<btCollisionShape> s_collisionShape;
		PhysicsSimulator* m_simulator;
	
	public:
		DummyActor(PhysicsSimulator* simulator);
		~DummyActor();
};

#endif
