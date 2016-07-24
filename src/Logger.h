#ifndef LOGGER_H
#define LOGGER_H

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

class Logger {
	private:
	std::string   timestamp();
	std::ofstream logFile;

	public:
	enum LogLevel { INFO, DEBUG, WARNING, ERROR, CRITICAL };

	Logger();
	~Logger();

	void write(LogLevel level, const std::string& msg);
};

extern Logger* g_logger;

#endif
