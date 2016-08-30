#include "InputModel.hpp"

// TODO: Handle multiple keys in a binding
// TODO: Handle multiple functions in a binding
void InputModel::bind(int key, std::function<void()> callback) {
	assert(m_bindings.count(key) == 0);
	m_bindings.emplace(key, callback);
}

void InputModel::update(GLFWwindow& window) {
	for(auto pair : m_bindings)
		if(glfwGetKey(&window, pair.first) == GLFW_PRESS) pair.second();
}
