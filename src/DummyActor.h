#ifndef DUMMYACTOR_H
#define DUMMYACTOR_H

#include "Actor.h"
#include "RenderModel2D.h"
#include <GL/glew.h>
#include <memory>
#include "Logger.h"
#include <stdexcept>

class DummyActor : public Actor {
	private:
		static std::unique_ptr<btCollisionShape> s_collisionShape;
	
	public:
		DummyActor();
		~DummyActor();
};

#endif
