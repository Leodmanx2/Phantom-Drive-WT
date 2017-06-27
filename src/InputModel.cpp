#include "InputModel.hpp"

InputModel* InputModel::activeModel = nullptr;

using namespace std::placeholders;

InputModel::InputModel(const std::string& schemaName)
  : m_firstMousePoll(true), m_schema(schemaName) {
	try {
		std::string fileName = SCHEMA_DIR + schemaName + ".pro";
		PlCall("pd_consult", PlTerm(fileName.c_str()));
	} catch(const PlException& exception) {
		g_logger.write(Logger::LogLevel::LOG_ERROR, static_cast<char*>(exception));
	}
}

void InputModel::update(GLFWwindow& window) {
	// TODO: Handle multiple keys in a binding
	activeModel = this;
	try {
		// Process mouse movement
		glfwGetCursorPos(&window, &m_newMousePos.x, &m_newMousePos.y);
		if(m_firstMousePoll) {
			m_oldMousePos    = m_newMousePos;
			m_firstMousePoll = false;
		}
		PlCall("schema_handleMouse",
		       {m_schema.c_str(),
		        m_oldMousePos.x,
		        m_oldMousePos.y,
		        m_newMousePos.x,
		        m_newMousePos.y});
		m_oldMousePos = m_newMousePos;
	} catch(const PlException& exception) {
		g_logger.write(Logger::LogLevel::LOG_ERROR, static_cast<char*>(exception));
	}

	activeModel = nullptr;
}

void InputModel::process(const KeyEvent& event) {
	// TODO: Handle multiple keys in a binding
	activeModel = this;
	try {
		PlCall("key_bind",
		       {m_schema.c_str(),
		        static_cast<long>(event.key),
		        static_cast<long>(event.action),
		        static_cast<long>(event.modifiers)});
	} catch(const PlException& exception) {
		g_logger.write(Logger::LogLevel::LOG_ERROR, static_cast<char*>(exception));
	}
	activeModel = nullptr;
}

void InputModel::process(const MouseButtonEvent& event) {
	// TODO: Handle multiple keys in a binding
	activeModel = this;
	try {
		PlCall("key_bind",
		       {m_schema.c_str(),
		        static_cast<long>(event.button),
		        static_cast<long>(event.action),
		        static_cast<long>(event.modifiers)});
	} catch(const PlException& exception) {
		g_logger.write(Logger::LogLevel::LOG_ERROR, static_cast<char*>(exception));
	}
	activeModel = nullptr;
}

void InputModel::process(const MouseMovementEvent& event) {
	activeModel = this;
	try {
		PlCall("schema_handleMouse", {m_schema.c_str(), event.dx, event.dy});
	} catch(const PlException& exception) {
		g_logger.write(Logger::LogLevel::LOG_ERROR, static_cast<char*>(exception));
	}
	activeModel = nullptr;
}
