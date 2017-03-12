#include "GridMap.h"
#include <iostream>
#include <fstream>

namespace Robot {
	GridCoordinate::GridCoordinate(): x(0), y(0) {}
	GridCoordinate::GridCoordinate(int x, int y): x(x), y(y) {}
	GridCoordinate::GridCoordinate(const GridCoordinate& c): x(c.x), y(c.y) {}
	
	bool GridCoordinate::operator== (const GridCoordinate& other) {
		return (x == other.x) && (y == other.y);
	}
	
	std::string GridCoordinate::toString() {
		char buf[100];
    snprintf(buf, 100, "[ x=%d y=%d ]", x, y);
    return std::string(buf);
	}

	GridMap::GridMap(): width(0), height(0), resolution(0) {
		for (int i = 0; i < MAX_ROWS; i ++) {
			for (int j = 0; j < MAX_COLS; j ++) {
				grid[i][j] = 0;
			}
		}
	}

	void GridMap::readFromFile(std::string fileName, float res, int scale) {
		char nextChar;

    std::ifstream inFile(fileName);

    // Skip the first line
    inFile.getline(fileHeader,80);
    // Read the map definition
    inFile >> width >> height >> maxVal;
    // resolution = res / scale;

    /* Read in map; */
    for (int i = 0; i < height; i ++) {
			for (int j = 0; j < width; j ++) {
	  		inFile >> nextChar;
	  		if (!nextChar) {
			    grid[i/scale][j/scale] = 1;
	  		}
	  	}
    }
    width = width / scale;
    height = height / scale;

    resolution = res * scale;
    resolutionX = resolution;
    resolutionY = resolution;
	}

	void GridMap::readFromFile(std::string fileName, float resX, float resY, int scale) {
		char nextChar;

    std::ifstream inFile(fileName);

    // Skip the first line
    inFile.getline(fileHeader,80);
    // Read the map definition
    inFile >> width >> height >> maxVal;
    // resolution = res / scale;

    /* Read in map; */
    for (int i = 0; i < height; i ++) {
			for (int j = 0; j < width; j ++) {
	  		inFile >> nextChar;
	  		if (!nextChar) {
			    grid[i/scale][j/scale] = 1;
	  		}
	  	}
    }
    width = width / scale;
    height = height / scale;

    resolutionX = resX * scale;
    resolutionY = resY * scale;
    resolution = (resolutionX + resolutionX) / 2;
	}

	void GridMap::writeToFile(std::string fileName) {
		std::ofstream outFile(fileName);
    outFile << fileHeader << std::endl;
    outFile << width << " " << height << std::endl
	    << maxVal << std::endl;

    for (int i=0; i < height; i++) {
			for (int j = 0; j < width; j++) {
	    	outFile << (char)((grid[i][j] - 1) * 3);
	    }
    }
	}

	std::string GridMap::toString(int limit) {
		std::string result = "";
		char buf[100];
		snprintf(buf, 100, "res=%.3f; width=%.2f(%d); height=%.2f(%d)\n", resolution, resolution*width, width, resolution*height, height);
		result = result + buf;

		for (int i = 0; i < height && i < limit; i ++) {
			for (int j = 0; j < width && j < limit; j ++) {
				char buf[10];
				snprintf(buf, 10, " %d", grid[i][j]);
				result = result + buf;
			}
			result = result + "\n";
		}
		return result;
	}

	char GridMap::getValue(GridCoordinate g) {
		return grid[g.y][g.x];
	}

	void GridMap::setValue(GridCoordinate g, char value) {
		grid[g.y][g.x] = value;
	}

	bool GridMap::outofRange(GridCoordinate g) {
		return (g.x >= width || g.y >= height) || (g.x < 0 || g.y < 0);
	}

	int GridMap::getIndex(GridCoordinate g) {
		return g.y * width + g.x;
	}

	GridCoordinate GridMap::coordinateToGrid2(const Coordinate c) {
		Coordinate gridOrigin;
		gridOrigin.x = - width * resolution / 2;
		gridOrigin.y = height * resolution / 2;

		GridCoordinate g;
		g.x = (int)((c.x - gridOrigin.x)/resolution + 0.5);
		g.y = -(int)((c.y - gridOrigin.y)/resolution + 0.5);
		return g;
	}

	GridCoordinate GridMap::coordinateToGrid(const Coordinate c) {
		Coordinate gridOrigin;
		gridOrigin.x = - width * resolutionX / 2;
		gridOrigin.y = height * resolutionY / 2;

		GridCoordinate g;
		g.x = (int)((c.x - gridOrigin.x)/resolutionX + 0.5);
		g.y = -(int)((c.y - gridOrigin.y)/resolutionY + 0.5);
		return g;
	}

	Coordinate GridMap::gridToCoordinate2(const GridCoordinate g) {
		Coordinate gridOrigin;
		gridOrigin.x = - width * resolution / 2;
		gridOrigin.y = height * resolution / 2;

		Coordinate c;
		c.x = gridOrigin.x + (g.x + 0.5) * resolution;
		c.y = gridOrigin.y - (g.y + 0.5) * resolution;
		return c;
	}

	Coordinate GridMap::gridToCoordinate(const GridCoordinate g) {
		Coordinate gridOrigin;
		gridOrigin.x = - width * resolutionX / 2;
		gridOrigin.y = height * resolutionY / 2;

		Coordinate c;
		c.x = gridOrigin.x + (g.x + 0.5) * resolutionX;
		c.y = gridOrigin.y - (g.y + 0.5) * resolutionY;
		return c;
	}

	int GridMap::getObstacleDensity(GridCoordinate g) {
		int density = 0;
		for (int x = g.x - 1; x <= g.x + 1; x ++) {
			for (int y = g.y - 1; y <= g.y + 1; y ++) {
				GridCoordinate node(x, y);
				if (node == g) continue;
				if (outofRange(node) || getValue(node) == 1) density ++;
			}
		}
		return density;
	}

}
