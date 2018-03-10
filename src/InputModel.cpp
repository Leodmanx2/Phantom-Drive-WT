#include "InputModel.hpp"

#include "input.hpp"
#include <plog/Log.h>

using namespace plog;

InputModel::InputModel(const std::string& schemaName) : m_schema(schemaName) {
	try {
		std::string fileName = SCHEMA_DIR + schemaName + ".pro";
		PlCall("pd_consult", PlTerm(fileName.c_str()));
	} catch(const PlException& exception) {
		LOG(error) << static_cast<char*>(exception);
	}
}

void InputModel::update(GLFWwindow&) {}

void InputModel::process(SpatialModel& spatial, const KeyEvent& event) {
	// TODO: Handle multiple keys in a binding
	try {
		PlCall("key_bind",
		       {&spatial,
		        m_schema.c_str(),
		        static_cast<long>(event.key),
		        static_cast<long>(event.action),
		        static_cast<long>(event.modifiers)});
	} catch(const PlException& exception) {
		LOG(error) << static_cast<char*>(exception);
	}
}

void InputModel::process(SpatialModel& spatial, const MouseButtonEvent& event) {
	// TODO: Handle multiple keys in a binding
	try {
		PlCall("key_bind",
		       {&spatial,
		        m_schema.c_str(),
		        static_cast<long>(event.button),
		        static_cast<long>(event.action),
		        static_cast<long>(event.modifiers)});
	} catch(const PlException& exception) {
		LOG(error) << static_cast<char*>(exception);
	}
}

void InputModel::process(SpatialModel&             spatial,
                         const MouseMovementEvent& event) {
	try {
		// TODO: give model address
		PlCall("schema_handleMouse",
		       {&spatial, m_schema.c_str(), event.dx, event.dy});
	} catch(const PlException& exception) {
		LOG(error) << static_cast<char*>(exception);
	}
}
