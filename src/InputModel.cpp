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
		// Get keys bound to schema
		// Binding fills the role of the anonymous variable _, since
		// the API doesn't offer any special functionality for it.
		PlTerm     Key, Bag, Binding;
		PlCompound skb("schema_key_binding", {m_schema.c_str(), Key, Binding});
		PlCall("findall", {Key, skb, Bag});

		// Check all the keys and call their bindings if pressed
		PlTail tail(Bag);
		while(tail.next(Key)) {
			if(glfwGetKey(&window, Key) == GLFW_PRESS) {
				PlTerm Callback;
				PlCall("schema_key_binding", {m_schema.c_str(), Key, Callback});
				PlCall(static_cast<char*>(Callback));
			}
		}

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
