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

void Actor::draw(float* viewMatrix, float* projectionMatrix) {
	// TODO: Calculate up-to-date model matrix
	m_renderModel->draw(modelMatrix, viewMatrix, projectionMatrix);
}
