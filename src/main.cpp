#include "Application.hpp"

#include "Logger.hpp"

int main(int argc, char* argv[]) {
	g_logger->write(Logger::LOG_INFO, "Starting program");

	try {
		char*    plArgv[] = {argv[0]};
		PlEngine engine(1, plArgv);
		PlCall("consult", PlTerm("core"));

		Application app(argc, argv);
		app.run();
	} catch(const std::exception& exception) {
		g_logger->write(Logger::LOG_CRITICAL, exception.what());
		g_logger->write(Logger::LOG_INFO, "Exited program unsuccessfully");
		return EXIT_FAILURE;
	}

	g_logger->write(Logger::LOG_INFO, "Exited program successfully");
	return EXIT_SUCCESS;
}
