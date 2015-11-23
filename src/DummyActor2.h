#ifndef DUMMYACTOR2_H
#define DUMMYACTOR2_H

#include "Actor.h"
#include "RenderModel3D.h"
#include <GL/glew.h>
#include <memory>
#include "Logger.h"
#include <stdexcept>
#include "PhysicsSimulator.h"
#include "ActorMotionState.h"

class DummyActor2 : public Actor {
	public:
		DummyActor2();
		~DummyActor2();
};

#endif
