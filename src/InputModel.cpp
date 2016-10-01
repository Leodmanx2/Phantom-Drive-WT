#include "InputModel.hpp"

InputModel* InputModel::activeModel = nullptr;

// Takes the upper-case ASCII key code as the first argument and the
// callback as the second.
PREDICATE(pd_bindKey, 2) {
	try {
		int         key    = static_cast<long>(A1);
		const char* handle = static_cast<char*>(A2);
		InputModel::activeModel->bindKey(key, [=]() { PlCall(handle); });
	} catch(const PlException& exception) {
		g_logger->write(Logger::LOG_ERROR, static_cast<char*>(exception));
	}
	return true;
}

// Takes the schema identifier as argument
PREDICATE(pd_bindMouse, 1) {
	try {
		const char* schema    = static_cast<char*>(A1);
		std::string mousePred = std::string(schema) + "_mouseHandle";
		InputModel::activeModel->bindMouse(
		  [=](glm::dvec2 lastPos, glm::dvec2 newPos) {
			  PlCall(mousePred.c_str(),
			         PlTermv(lastPos.x, lastPos.y, newPos.x, newPos.y));
			});
	} catch(const PlException& exception) {
		g_logger->write(Logger::LOG_ERROR, static_cast<char*>(exception));
	}
	return true;
}

InputModel::InputModel(const std::string& schemaName) : m_firstMousePoll(true) {
	try {
		std::string fileName = SCHEMA_DIR + schemaName + ".pro";
		std::string keyPred  = schemaName + "_bindKeys";
		PlCall("pd_consult", PlTerm(fileName.c_str()));
		activeModel = this;
		PlCall(keyPred.c_str());
		PlCall("pd_bindMouse", PlTerm(schemaName.c_str()));
		activeModel = nullptr;
	} catch(const PlException& exception) {
		g_logger->write(Logger::LOG_ERROR, static_cast<char*>(exception));
	}
}

// TODO: Handle multiple keys in a binding
void InputModel::bindKey(int key, std::function<void()> callback) {
	assert(m_keyBindings.count(key) == 0);
	m_keyBindings.emplace(key, callback);
}

void InputModel::bindMouse(
  std::function<void(glm::dvec2 lastPos, glm::dvec2 newPos)> callback) {
	m_mouseBinding = callback;
}

void InputModel::update(GLFWwindow& window) {
	for(auto pair : m_keyBindings)
		if(glfwGetKey(&window, pair.first) == GLFW_PRESS) pair.second();

	// Process mouse position
	if(m_mouseBinding) {
		glfwGetCursorPos(&window, &m_newMousePos.x, &m_newMousePos.y);
		if(m_firstMousePoll) {
			m_oldMousePos    = m_newMousePos;
			m_firstMousePoll = false;
		}
		m_mouseBinding(m_oldMousePos, m_newMousePos);
		m_oldMousePos = m_newMousePos;
	}
}
