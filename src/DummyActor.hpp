#ifndef DUMMYACTOR_H
#define DUMMYACTOR_H

#include "Actor.hpp"
#include "ActorMotionState.hpp"
#include "Logger.hpp"
#include "PhysicsSimulator.hpp"
#include "RenderModel.hpp"
#include <GL/glew.h>
#include <memory>
#include <stdexcept>

class DummyActor : public Actor {
	public:
	DummyActor();
};

#endif
