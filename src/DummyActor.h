#ifndef DUMMYACTOR_H
#define DUMMYACTOR_H

#include "Actor.h"
#include "RenderModel.h"
#include <GL/glew.h>
#include <memory>
#include "Logger.h"
#include <stdexcept>
#include "PhysicsSimulator.h"
#include "ActorMotionState.h"

class DummyActor : public Actor {
	public:
		DummyActor();
};

#endif
