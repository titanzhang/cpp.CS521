#include "Pilot.h"
#include <cmath>
#include "Logger.h"

namespace Robot {

	Pilot::Pilot(Coordinate* wayPoints, int size): mSize(size), mIndex(0) {
		for (int i = 0; i < size; i ++) {
			mList[i] = wayPoints[i];
			Logger::debug("Pilot", mList[i].toString());
		}
	}

	bool Pilot::hasNext() {
		return mIndex < mSize;
	}

	Coordinate Pilot::next() {
		return mList[mIndex ++];
	}

	
}