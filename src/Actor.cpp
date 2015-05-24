#include "Actor.h"

Actor::Actor() {
	
}

Actor::~Actor() {
	delete m_renderModel;
}

void Actor::translate(float longitude, float latitude, float altitude) {
	
}

void Actor::rotate(float roll, float pitch, float yaw) {
	
}

void Actor::update() {
	
}

void Actor::draw() {
	m_renderModel->draw();
}
