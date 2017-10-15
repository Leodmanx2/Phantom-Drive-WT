#ifndef INPUTMODEL_H
#define INPUTMODEL_H

#define GLFW_INCLUDE_NONE

#include "Logger.hpp"
#include "SpatialModel.hpp"
#include "input.hpp"
#include "utility.hpp"
#include <GLFW/glfw3.h>
#include <SWI-cpp.h>
#include <cassert>
#include <functional>
#include <glm/glm.hpp>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <vector>

class InputModel {
	const std::string SCHEMA_DIR = "Controls/";

	private:
	std::string m_schema;

	public:
	explicit InputModel(const std::string& schemaName);

	void update(GLFWwindow& window);
	void process(SpatialModel& spatial, const KeyEvent& event);
	void process(SpatialModel& spatial, const MouseButtonEvent& event);
	void process(SpatialModel& spatial, const MouseMovementEvent& event);
};

#endif
