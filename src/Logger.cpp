#include "Logger.hpp"

Logger::Logger() { logFile.open(timestamp()); }

Logger::~Logger() { logFile.close(); }

const std::string Logger::timestamp() {
	const time_t timeNow = time(nullptr);
	struct tm    localTimeNow;
	localtime_s(&localTimeNow, &timeNow);
	char buffer[80];
	std::strftime(
	  buffer, sizeof(buffer), "log/%Y-%m-%W_%H-%M-%S.log", &localTimeNow);
	return std::string(buffer);
}

/*
 * Writes a message to both standard output and a process-specific log file named with a timestamp from the beginning of the process.
 *
 * @param [in] level  The type of message. Determines how printing may be done. One of: INFO, DEBUG, WARNING, ERROR, CRITICAL.
 * @param [in] msg    The message to write
 */
void Logger::write(LogLevel level, const std::string& msg) {
	switch(level) {
		case LogLevel::LOG_INFO:
			std::cout << "[INFO]  " << msg << std::endl;
			logFile << "[INFO] ";
			break;
		case LogLevel::LOG_DEBUG:
			std::clog << "[DEBUG] " << msg << std::endl;
			logFile << "[DEBUG] ";
			break;
		case LogLevel::LOG_WARNING:
			std::cerr << "[WARNING]  " << msg << std::endl;
			logFile << "[WARNING] ";
			break;
		case LogLevel::LOG_ERROR:
			std::cerr << "[ERROR] " << msg << std::endl;
			logFile << "[ERROR] ";
			break;
		case LogLevel::LOG_CRITICAL:
			std::cerr << "[CRITICAL] " << msg << std::endl;
			logFile << "[CRITICAL] ";
			break;
	}
	logFile << msg << std::endl;
}

Logger g_logger;
