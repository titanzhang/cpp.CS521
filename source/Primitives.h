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
		Pose operator+ (const Pose& other);
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

	class RangerReading {
	public:
		RangerReading();
		RangerReading(const RangerReading& r);
		void add(double reading);
		Pose get(int index);
		Pose getNormValue(int index);
		int size();
		std::string toString();
	private:
		static const int MAX_LIST_NUMBER = 100;
		double mList[MAX_LIST_NUMBER];
		int mIndex;
		int mSize;
		double sensorList[8] = {90,50,30,10,-10,-30,-50,-90};
	};
}

#endif