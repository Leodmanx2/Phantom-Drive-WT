#include "Logger.h"

Logger::Logger() {
	logFile.open(timestamp());
}

Logger::~Logger() {
	logFile.close();
}

std::string Logger::timestamp() {
	time_t timeNow = time(NULL);
	struct tm localTimeNow = *localtime(&timeNow);
	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%W_%H-%M-%S.log", &localTimeNow);
	return std::string(buffer);
}

void Logger::write(LogLevel level, const char* msg) {
	switch(level) {
		case INFO:
			std::cout << "[INFO]  ";
			logFile << "[INFO] ";
			break;
		case DEBUG:
			std::cout << "[DEBUG] ";
			logFile << "[DEBUG] ";
			break;
		case WARNING:
			std::cout << "[WARNING]  ";
			logFile << "[WARNING] ";
			break;
		case ERROR:
			std::cout << "[ERROR] ";
			logFile << "[ERROR] ";
			break;
		case CRITICAL:
			std::cout << "[CRITICAL] ";
			logFile << "[CRITICAL] ";
			break;
	}
	std::cout << msg << std::endl;
	logFile << msg << std::endl;
}

Logger* g_logger = new Logger();
