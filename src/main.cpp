#include "Application.h"

#include "Logger.h"

int main(int argc, char* argv[]) {
	g_logger->write(Logger::INFO, "Starting program");
	
	g_logger->write(Logger::INFO, "Initializing virtual file system");
	if(PHYSFS_init(argv[0]) == 0) {
		g_logger->write(Logger::CRITICAL, PHYSFS_getLastError());
		g_logger->write(Logger::INFO, "Exited program unsuccessfully");
		return EXIT_FAILURE;
	}
	
	try {
		Application app;
		app.run();
	}
	catch(const std::runtime_error& exception) {
		g_logger->write(Logger::CRITICAL, exception.what());
		g_logger->write(Logger::INFO, "Exited program unsuccessfully");
		return EXIT_FAILURE;
	}
	
	g_logger->write(Logger::INFO, "Shutting down virtual file system");
	PHYSFS_deinit();
	
	g_logger->write(Logger::INFO, "Exited program successfully");
	return EXIT_SUCCESS;
}

