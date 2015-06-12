#include "Actor.h"

Actor::Actor() {
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_left = glm::vec3(-1.0f, 0.0f, 0.0f);
}

Actor::~Actor() {
	delete m_renderModel;
}

void Actor::translate(float longitude, float latitude, float altitude) {
	glm::vec3 longVec = m_forward * longitude;
	glm::vec3 latVec = m_left * latitude;
	glm::vec3 altVec = m_up * altitude;
	
	m_position += longVec + latVec + altVec;
}

void Actor::rotate(float roll, float pitch, float yaw) {
	glm::quat rotationQuat(glm::vec3(pitch, yaw, roll));
	m_orientation *= rotationQuat;
	
	canonicalForward = glm::vec3(0.0f, 0.0f, -1.0f);
	canonicalUp = glm::vec3(0.0f, 1.0f, 0.0f);
	canonicalLeft = glm::vec3(-1.0f, 0.0f, 0.0f);

	glm::mat4 orientationMatrix = glm::mat4_cast(m_orientation);
	m_forward = orientationMatrix * canonicalForward;
	m_up = orientationMatrix * canonicalUp;
	m_left = orientationMatrix * canonicalLeft;
}

void Actor::update() {
	
}

void Actor::draw(float* viewMatrix, float* projectionMatrix) {
	glm::mat4 rotationMatrix = glm::mat4_cast(m_orientation);
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_position);
	
	glm::mat4 modelMatrix = translationMatrix * roationMatrix;
	
	m_renderModel->draw(glm::value_ptr(modelMatrix), 
	                    viewMatrix, 
											projectionMatrix);
}
