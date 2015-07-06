#ifndef DUMMYACTOR_H
#define DUMMYACTOR_H

#include "Actor.h"
#include "RenderModel2D.h"
#include <GL/glew.h>

class DummyActor : public Actor {
	private:
		RenderModel2D* m_model;
	
	public:
		DummyActor();
		~DummyActor();
};

#endif
