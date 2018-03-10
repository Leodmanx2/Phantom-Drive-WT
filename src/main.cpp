#include "Application.hpp"

#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Log.h>

using namespace plog;

void glfw_error_callback(int, const char* description) {
	LOG(error) << description;
}

int main(int argc, char* argv[]) {
	RollingFileAppender<TxtFormatter>  fileAppender("log/log.txt", 1000000, 2);
	ColorConsoleAppender<TxtFormatter> consoleAppender;
	plog::init(debug, &fileAppender).addAppender(&consoleAppender);
	LOG(info) << "starting program";

	glfwSetErrorCallback(glfw_error_callback);
	if(!glfwInit()) {
		LOG(fatal) << "GLFW initialization failed";
		return EXIT_FAILURE;
	}

	try {
		const char* plArgv[] = {argv[0], "--quiet"};
		LOG(info) << "starting Prolog engine";
		PlEngine engine(2, const_cast<char**>(plArgv));

		Application app(argc, argv);
		app.run();
	} catch(const std::exception& exception) {
		LOG(fatal) << exception.what();
		LOG(info) << "exited program unsuccessfully";
		return EXIT_FAILURE;
	}

	glfwTerminate();
	LOG(info) << "exited program successfully";
	return EXIT_SUCCESS;
}
