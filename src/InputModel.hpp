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
	std::map<int, std::function<void()>> m_keyBindings;
	std::function<void(glm::dvec2 lastPos, glm::dvec2 newPos)> m_mouseBinding;

	glm::dvec2 m_oldMousePos;
	glm::dvec2 m_newMousePos;
	bool       m_firstMousePoll;

	public:
	explicit InputModel(const std::string& schemaName);

	static InputModel* activeModel;

	void bindKey(int key, std::function<void()> callback);
	void bindMouse(std::function<void(glm::dvec2, glm::dvec2)> callback);
	void update(GLFWwindow& window);
};

#endif
