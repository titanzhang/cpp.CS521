#ifndef __WAVEFRONT_H__
#define __WAVEFRONT_H__

#include "Primitives.h"
#include "GridMap.h"

namespace Robot {

	class WaveFront {
	public:
		WaveFront(Coordinate start, Coordinate end, GridMap * map);
		bool calculatePath();
		GridCoordinate * getWaypoints(int * size);
		void markPath();
		void skipSmooth(bool skip);

	private:
		GridMap * pMap;
		Coordinate start;
		Coordinate end;
		static const int MAX_WAYPOINTS = 1000;
		int numWaypoints;
		GridCoordinate wayPoints[MAX_WAYPOINTS];
		bool bSkipSmooth = false;

		GridCoordinate getNextWP(GridCoordinate current);
		bool smooth(GridCoordinate * wp, int num, GridCoordinate * wpSmooth, int& numSmooth);
		bool hasObstacle(GridCoordinate wp1, GridCoordinate wp2);
		bool propagation();
		void generateWaypoints(GridCoordinate * wp, int& numWp);
	};

	class WaveValue {
	public:
		WaveValue(const GridCoordinate& g, char value);
		WaveValue(const WaveValue& w);

		GridCoordinate g;
		char v;
	};
}

#endif