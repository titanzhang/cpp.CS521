#ifndef __NAVIGATOR_H__
#define __NAVIGATOR_H__

#include <string>
#include "Primitives.h"
#include "GridMap.h"

namespace Robot {

	class Navigator {
	public:
		Navigator();
		void readMap(std::string fileName, float resX, float resY, int scale);
		bool planPath(Coordinate start, Coordinate end);
		Coordinate * getWaypoints(int* size);

	private:
		static const int MAX_LIST_NUMBER = 1000;
		Coordinate mList[MAX_LIST_NUMBER];
		int mSize;
		GridMap * pMap;
	};

}

#endif