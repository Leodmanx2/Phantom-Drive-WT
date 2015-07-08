#ifndef DUMMYACTOR_H
#define DUMMYACTOR_H

#include "Actor.h"
#include "RenderModel2D.h"
#include <GL/glew.h>

#include "Logger.h"
#include <stdexcept>

class DummyActor : public Actor {
	public:
		DummyActor();
		~DummyActor();
};

#endif
