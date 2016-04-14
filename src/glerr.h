#ifndef GLERR_H
#define GLERR_H

#include "Logger.h"
#include <sstream>
#include <GL/glew.h>

#ifdef LOG_GL
inline void glLogErr(const std::string& context) {
	std::stringstream ss;
	ss << context << ": ";
	bool write = true;
	switch(glGetError()) {
		case GL_NO_ERROR:
			#ifndef LOG_GL_LOGNOERR
			write = false;
			#endif
			ss << "No error";
			break;
		case GL_INVALID_ENUM:
			ss << "An unacceptable value is specified for an enumerated argument.";
			break;
		case GL_INVALID_VALUE:
			ss << "A numeric argument is out of range.";
			break;
		case GL_INVALID_OPERATION:
			ss << "The specified operation is not allowed in the current state.";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			ss << "The framebuffer object is not complete.";
			break;
		case GL_OUT_OF_MEMORY:
			ss << "There is not enough memory left to execute the command.";
			break;
		case GL_STACK_UNDERFLOW:
			ss << "An attempt has been made to perform an operation that would cause an internal stack to underflow.";
			break;
		case GL_STACK_OVERFLOW:
			ss << "An attempt has been made to perform an operation that would cause an internal stack to overflow.";
			break;
		default:
			ss << "Unknown error!";
	}
	if(write) g_logger->write(Logger::DEBUG, ss.str());
}
#else
inline void glLogErr(const std::string& context) {return;}
#endif

#endif
