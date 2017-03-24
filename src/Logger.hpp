#ifndef LOGGER_H
#define LOGGER_H

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#define WRITE(MSG) g_logger.write(Logger::LOG_DEBUG, MSG);

class Logger final {
	private:
	std::string   timestamp();
	std::ofstream logFile;

	public:
	enum LogLevel { LOG_INFO, LOG_DEBUG, LOG_WARNING, LOG_ERROR, LOG_CRITICAL };

	Logger();
	~Logger();

	void write(LogLevel level, const std::string& msg);
};

extern Logger g_logger;

#endif
