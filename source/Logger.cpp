#include "Logger.h"
#include <iostream>

#define DEBUG_ENABLED

namespace Robot {
	void Logger::debug(std::string module, std::string message) {
		#ifdef DEBUG_ENABLED
		std::cout << module << ": " << message << std::endl;
		#endif
	}

	void Logger::info(std::string module, std::string message) {
		std::cout << module << ": " << message << std::endl;
	}
}