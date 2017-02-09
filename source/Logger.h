#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>

namespace Robot {

	class Logger {
	public:
		static void debug(std::string module, std::string message);
		static void info(std::string module, std::string message);
	};
}

#endif