#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__

#include <string>
#include <exception>

namespace Robot {

	extern const double PI;

	class Coordinate {
	public:
		Coordinate();
		Coordinate(double x, double y);
		Coordinate(const Coordinate& c);
		double distanceTo(const Coordinate& c) const;
		double distanceTo(double x, double y) const;
		double angle(const Coordinate &p) const;
		double angle(double x, double y) const;
		std::string toString();
		double x;
		double y;
	};

	class Pose {
	public:
		Pose();
		Pose(double yaw, double speed);
		Pose(const Pose& p);
		std::string toString();
		double yaw;
		double speed;
	};

	class Angle {
	public:
		static double normalize(double a);
	};

	class RobotException: public std::exception {
	public:
		RobotException(const std::string &message) throw();
		const char * what() const throw();

	private:
		std::string mUserMessage;
	};
}

#endif