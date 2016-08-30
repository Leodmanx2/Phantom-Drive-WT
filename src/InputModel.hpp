#ifndef INPUTMODEL_H
#define INPUTMODEL_H

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <cassert>
#include <functional>
#include <map>
#include <vector>

class InputModel {
	private:
	std::map<int, std::function<void()>> m_bindings;

	public:
	void bind(int key, std::function<void()> callback);
	void update(GLFWwindow& window);
};

#endif
