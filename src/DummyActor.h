#ifndef DUMMYACTOR_H
#define DUMMYACTOR_H

#include "SimulatedPhysicsActor.h"
#include "RenderModel2D.h"
#include <GL/glew.h>
#include <memory>
#include "Logger.h"
#include <stdexcept>
#include "PhysicsSimulator.h"
#include "ActorMotionState.h"

class DummyActor : public SimulatedPhysicsActor {
	protected:
		// Should be static, so must be declared by child.
		// TODO: Look into making sure this variable is declared while maintaining static-ness
		static std::unique_ptr<btCollisionShape> s_collisionShape;
	
	public:
		explicit DummyActor(PhysicsSimulator* simulator);
		~DummyActor();
};

#endif
