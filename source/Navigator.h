#ifndef __NAVIGATOR_H__
#define __NAVIGATOR_H__

#include <string>
#include "Primitives.h"

namespace Robot {

	class Navigator {
	public:
		Navigator();
		bool initFromFile(std::string fileName) throw(RobotException);
		bool hasNext();
		Coordinate next();
		bool addWayPoint(double x, double y);

	private:
		static const int MAX_LIST_NUMBER = 100;
		Coordinate mList[MAX_LIST_NUMBER];
		int mIndex;
		int mSize;
	};

}

#endif