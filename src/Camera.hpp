#ifndef PD_CAMERA_HPP
#define PD_CAMERA_HPP

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
	void             translate(float longitude, float latitude, float altitude);
	void             rotate(float roll, float pitch, float yaw);
	void             setOrientation(float deg, float x, float y, float z);
	void             setPosition(float x, float y, float z);
	const glm::vec4& position() const;
	const glm::quat& orientation() const;
};

#endif
