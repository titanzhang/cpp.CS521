#include <cmath>
#include <cerrno>
#include <cstring>
#include "Primitives.h"

namespace Robot {

	const double PI = 3.1415926535;

	// ***********************************
	// Implementation of class Coordinate
	// ***********************************
	Coordinate::Coordinate(): x(0), y(0) {}
	Coordinate::Coordinate(double x, double y): x(x), y(y) {}
	Coordinate::Coordinate(const Coordinate& c): x(c.x), y(c.y) {}

	double Coordinate::distanceTo(const Coordinate& c) const {
		return this->distanceTo(c.x, c.y);
	}

	double Coordinate::distanceTo(double x, double y) const {
		return std::hypot(this->x - x, this->y - y);
	}

	double Coordinate::angle(const Coordinate& c) const {
		return this->angle(c.x, c.y);
	}

	double Coordinate::angle(double x, double y) const {
    if (this->x == x && this->y == y) {
      return 0;
    }
    return std::atan2(y - this->y, x - this->x);		
	}

	std::string Coordinate::toString() {
    char buf[100];
    snprintf(buf, 100, "[ x=%.3f y=%.3f ]", x, y);
    return std::string(buf);
	}

	// ***********************************
	// Implementation of class Pose
	// ***********************************
	Pose::Pose(): yaw(0), speed(0) {}
	Pose::Pose(double yaw, double speed): yaw(yaw), speed(speed) {}
	Pose::Pose(const Pose& p): yaw(p.yaw), speed(p.speed) {}

	std::string Pose::toString() {
    char buf[100];
    snprintf(buf, 100, "[ yaw=%.3f speed=%.3f ]", yaw, speed);
    return std::string(buf);
	}

	// ***********************************
	// Implementation of class Angle
	// ***********************************
	double Angle::normalize(double a){
    while(a < -PI) a += 2.0*PI;
    while(a > PI) a -= 2.0*PI;  
    return a;		
	}

  // ----------------------------------------
  // Implementation of RobotException
  // ----------------------------------------
  RobotException::RobotException(const std::string &message) throw(): mUserMessage(message) {
    mUserMessage.append(": ");
    mUserMessage.append(std::strerror(errno));
  }

  const char * RobotException::what() const throw() {
    return mUserMessage.c_str();
  }
}