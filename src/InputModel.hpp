#ifndef INPUTMODEL_H
#define INPUTMODEL_H

#define GLFW_INCLUDE_NONE

#include "Logger.hpp"
#include "utility.hpp"
#include <GLFW/glfw3.h>
#include <SWI-cpp.h>
#include <cassert>
#include <functional>
#include <glm/glm.hpp>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

class InputModel {
	const std::string SCHEMA_DIR = "Controls/";

	private:
	glm::dvec2  m_oldMousePos;
	glm::dvec2  m_newMousePos;
	bool        m_firstMousePoll;
	std::string m_schema;

	public:
	explicit InputModel(const std::string& schemaName);

	static InputModel* activeModel;

	void update(GLFWwindow& window);
};

#endif
