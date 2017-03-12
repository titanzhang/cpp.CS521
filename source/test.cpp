#include <iostream>
#include "GridMap.h"
#include "WaveFront.h"

using namespace Robot;

int main(int argc, char **argv) {
	std::string directory = argv[0];
	int find = directory.find_last_of('/');
	directory = (find == std::string::npos)? "": directory.substr(0, find+1);
	std::string fileName = directory + "hospital_section.pnm";
	std::string outFileName = argv[1];
	float x = atof(argv[2]);
	float y = atof(argv[3]);
	Coordinate target(x, y);

	GridMap map;
	map.readFromFile(fileName, 0.0368, 0.0406, 10);
	// map.readFromFile(fileName, 0.04, 10);

	GridCoordinate g = map.coordinateToGrid(target);
	Coordinate c = map.gridToCoordinate(g);
	std::cout << target.toString() << " -> " << g.toString() << " -> " << c.toString() << std::endl;


	WaveFront planner(Coordinate(-10.071, 3.186), target, &map);
	if (!planner.calculatePath()) {
		std::cout << "No solution!" << std::endl;
	} else {
		std::cout << map.toString(0) << std::endl;

		int size;
		GridCoordinate * gridList = planner.getWaypoints(&size);
		for (int i = 0; i < size; i ++) {
			std::cout << gridList[i].toString() << " -> " << map.gridToCoordinate(gridList[i]).toString() << std::endl;
		}

		planner.markPath();
		map.writeToFile(outFileName);
	}

}