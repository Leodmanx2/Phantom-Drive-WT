#include "Application.h"

#include "Logger.h"
#include <sstream>

int main(int argc, char *argv[]) {
	g_logger->write(Logger::INFO, "Starting program");
	
	try {
		Application app;
		app.run();
	}
	catch(const std::runtime_error& exception) {
		g_logger->write(Logger::CRITICAL, exception.what());
		g_logger->write(Logger::INFO, "Exited program unsuccessfully");
		return EXIT_FAILURE;
	}
	
	g_logger->write(Logger::INFO, "Exited program successfully");
	return EXIT_SUCCESS;
}

