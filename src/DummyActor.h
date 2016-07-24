#ifndef DUMMYACTOR_H
#define DUMMYACTOR_H

#include "Actor.h"
#include "ActorMotionState.h"
#include "Logger.h"
#include "PhysicsSimulator.h"
#include "RenderModel.h"
#include <GL/glew.h>
#include <memory>
#include <stdexcept>

class DummyActor : public Actor {
	public:
	DummyActor();
};

#endif
