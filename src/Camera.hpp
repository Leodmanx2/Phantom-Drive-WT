#ifndef CAMERA_H
#define CAMERA_H

#include "InputModel.hpp"
#include "SpatialModel.hpp"
#include <SWI-cpp.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <queue>
#include <sstream>

// Forward declarations ------------------------------------------------------
struct KeyEvent;
struct MouseMovementEvent;
struct MouseButtonEvent;
// ---------------------------------------------------------------------------

class Camera {
	private:
	InputModel   m_inputModel;
	SpatialModel m_spatialModel;

	public:
	Camera();
	const glm::mat4 viewMatrix() const;

	void process(const KeyEvent& event);
	void process(const MouseButtonEvent& event);
	void process(const MouseMovementEvent& event);

	// Forwarding functions
	constexpr void translate(float longitude, float latitude, float altitude) {
		m_spatialModel.translate(longitude, latitude, altitude);
	}

	constexpr void rotate(float roll, float pitch, float yaw) {
		m_spatialModel.rotate(roll, pitch, yaw);
	}

	constexpr void setOrientation(float deg, float x, float y, float z) {
		m_spatialModel.setOrientation(deg, x, y, z);
	}

	constexpr void setPosition(float x, float y, float z) {
		m_spatialModel.setPosition(x, y, z);
	}

	constexpr const glm::vec4& position() const {
		return m_spatialModel.position();
	}

	constexpr const glm::quat& orientation() const {
		return m_spatialModel.orientation();
	}
};

#endif
