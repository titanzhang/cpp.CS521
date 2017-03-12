#ifndef __GRIDMAP_H__
#define __GRIDMAP_H__

#include <string>
#include "Primitives.h"

namespace Robot {
	class GridCoordinate {
	public:
		int x, y;
		GridCoordinate();
		GridCoordinate(int x, int y);
		GridCoordinate(const GridCoordinate& c);
		bool operator== (const GridCoordinate& other);
		std::string toString();
	};

	class GridMap {
	public:
		GridMap();
		void readFromFile(std::string fileName, float res, int scale);
		void readFromFile(std::string fileName, float resX, float resY, int scale);
		void writeToFile(std::string fileName);
		char getValue(GridCoordinate g);
		void setValue(GridCoordinate g, char value);
		bool outofRange(GridCoordinate g);
		int getIndex(GridCoordinate g);
		int getObstacleDensity(GridCoordinate g);
		GridCoordinate coordinateToGrid(const Coordinate c);
		Coordinate gridToCoordinate(const GridCoordinate g);
		GridCoordinate coordinateToGrid2(const Coordinate c);
		Coordinate gridToCoordinate2(const GridCoordinate g);

		std::string toString(int limit);

	private:
		static const int MAX_ROWS = 1000;
		static const int MAX_COLS = 1000;
		
		char grid[MAX_ROWS][MAX_COLS];
		int width;
		int height;
		float resolution;
		char fileHeader[80];
		int maxVal;

		float resolutionX;
		float resolutionY;
	};
}
#endif
