#include "Navigator.h"
#include <fstream>
#include <cstdio>
#include "Primitives.h"

namespace Robot {

		Navigator::Navigator(): mIndex(0), mSize(0) {}

		bool Navigator::initFromFile(std::string fileName) throw(RobotException) {
			mSize = 0;
			mIndex = 0;

			std::string line;
			std::ifstream file(fileName);
			if (file.is_open()) {
				while(std::getline(file, line)) {
					float x, y;
					int n = sscanf(line.c_str(), "%f\t%f", &x, &y);
					if (n < 2) { // Format error
						file.close();
						throw RobotException("Invalid waypoint file format");
						return false;
					}
					addWayPoint(x, y);		
				}
				file.close();
			} else { // Open file failed
				throw RobotException("Fail to open waypoint file");
				return false;
			}

			return true;
		}

		bool Navigator::hasNext() {
			return (mIndex < mSize);
		}

		Coordinate Navigator::next() {
			return mList[mIndex++];
		}

		bool Navigator::addWayPoint(double x, double y) {
			if (mSize >= MAX_LIST_NUMBER) {
				return false;
			}
			mList[mSize].x = x;
			mList[mSize].y = y;
			mSize ++;
			return true;
		}
	}
