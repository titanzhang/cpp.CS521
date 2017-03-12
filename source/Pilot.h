#ifndef __PILOT_H__
#define __PILOT_H__

#include <libplayerc++/playerc++.h>
#include "Primitives.h"

namespace Robot {

	class Pilot {
	public:
		Pilot(Coordinate* wayPoints, int size);
		bool hasNext();
		Coordinate next();

	private:
		static const int MAX_LIST_NUMBER = 1000;
		Coordinate mList[MAX_LIST_NUMBER];
		int mSize;
		int mIndex;
	};

}

#endif
