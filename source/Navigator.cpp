#include "Navigator.h"
#include <iostream>
#include <cstdio>
#include "Primitives.h"
#include "WaveFront.h"

namespace Robot {

		Navigator::Navigator(): mSize(0) {}

		void Navigator::readMap(std::string fileName, float resX, float resY, int scale) {
			pMap = new GridMap();
			pMap->readFromFile(fileName, resX, resY, scale);
		}

		bool Navigator::planPath(Coordinate start, Coordinate end) {
			WaveFront planner(start, end, pMap);
			if (!planner.calculatePath()) return false;

			GridCoordinate * wayPoints = planner.getWaypoints(&mSize);
			for (int i = 1; i < mSize; i ++) {
				mList[i - 1] = pMap->gridToCoordinate(wayPoints[i]);
			}
			mSize --;

			return true;
		}

		Coordinate * Navigator::getWaypoints(int* size) {
			*size = mSize;
			return mList;
		}
	}
