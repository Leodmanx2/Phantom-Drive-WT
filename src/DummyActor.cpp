#include "DummyActor.h"

DummyActor::DummyActor() {
	m_model = new RenderModel2D("", "ass/passthrough.vert.glsl", "ass/passthrough.frag.glsl");
}

DummyActor::~DummyActor() {
	delete m_model;
}
