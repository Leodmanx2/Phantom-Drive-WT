#include "Application.hpp"

#include "Logger.hpp"

void glfw_error_callback(int, const char* description) {
	g_logger.write(Logger::LOG_ERROR, description);
}

int main(int argc, char* argv[]) {
	g_logger.write(Logger::LOG_INFO, "Starting program");

	glfwSetErrorCallback(glfw_error_callback);
	if(!glfwInit()) { throw std::runtime_error("GLFW initialization failed"); }

	try {
		const char* plArgv[] = {argv[0], "--quiet"};
		g_logger.write(Logger::LOG_INFO, "Starting Prolog engine");
		PlEngine engine(2, const_cast<char**>(plArgv));

		Application app(argc, argv);
		app.run();
	} catch(const std::exception& exception) {
		g_logger.write(Logger::LOG_CRITICAL, exception.what());
		g_logger.write(Logger::LOG_INFO, "Exited program unsuccessfully");
		return EXIT_FAILURE;
	}

	glfwTerminate();
	g_logger.write(Logger::LOG_INFO, "Exited program successfully");
	return EXIT_SUCCESS;
}
