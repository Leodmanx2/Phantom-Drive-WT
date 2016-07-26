#include "Application.hpp"

#include "Logger.hpp"

int main(int argc, char* argv[]) {
	g_logger->write(Logger::INFO, "Starting program");

	try {
		Application app(argc, argv);
		app.run();
	} catch(const std::exception& exception) {
		g_logger->write(Logger::CRITICAL, exception.what());
		g_logger->write(Logger::INFO, "Exited program unsuccessfully");
		return EXIT_FAILURE;
	}

	g_logger->write(Logger::INFO, "Exited program successfully");
	return EXIT_SUCCESS;
}
