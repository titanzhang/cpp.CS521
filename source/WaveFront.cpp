#include "WaveFront.h"
#include "Logger.h"
#include <queue>
#include <set>
#include <cmath>
#include <iostream>

namespace Robot {
		WaveValue::WaveValue(const GridCoordinate& g, char value): g(g), v(value) { }
		WaveValue::WaveValue(const WaveValue& w): g(w.g), v(w.v) { }

		WaveFront::WaveFront(Coordinate start, Coordinate end, GridMap * map): start(start), end(end), pMap(map), numWaypoints(0) {}

		GridCoordinate * WaveFront::getWaypoints(int * size) {
			*size = numWaypoints;
			return wayPoints;
		}

		bool WaveFront::propagation() {
			GridCoordinate startGrid = pMap->coordinateToGrid(start);
			GridCoordinate endGrid = pMap->coordinateToGrid(end);

			// BFS propagation
			std::queue<WaveValue> queue;
			std::set<int> set;
			queue.push(WaveValue(startGrid, 2));

			while (!queue.empty()) {
				WaveValue v = queue.front();
				queue.pop();
				// std::cout << "Pop: " << v.g.toString() << std::endl;

				// Set value of current node
				pMap->setValue(v.g, v.v);
				if (v.g == endGrid) break;

				// Add neighbors to queue
				for (int x = v.g.x - 1; x <= v.g.x + 1; x ++) {
					for (int y = v.g.y - 1; y <= v.g.y + 1; y ++) {
						GridCoordinate neighbor(x, y);
						int neighborIndex = pMap->getIndex(neighbor);

						if (set.find(neighborIndex) != set.end()) continue;
						if (pMap->outofRange(neighbor)) continue;
						if (pMap->getValue(neighbor) != 0) continue;

						set.insert(neighborIndex);
						queue.push(WaveValue(neighbor, v.v + 1));
					}
				}
			}

			// Can not reach target
			return (pMap->getValue(endGrid) > 1);
		}

		void WaveFront::generateWaypoints(GridCoordinate * wp, int& numWp) {
			GridCoordinate startGrid = pMap->coordinateToGrid(start);
			GridCoordinate endGrid = pMap->coordinateToGrid(end);
			std::queue<GridCoordinate> wpQueue;

			// Extract way points
			GridCoordinate g = endGrid;
			while (!(g == startGrid)) {
				wpQueue.push(g);
				g = getNextWP(g);
			}
			numWp = wpQueue.size();

			// Flip the order of way points
			int i = numWp - 1;
			while (!wpQueue.empty()) {
				wp[i --] = wpQueue.front();
				wpQueue.pop();
			}
		}


		bool WaveFront::calculatePath() {
			// Wave front popagation
			if (!propagation()) {
				return false;
			}

			// Generate way points
			GridCoordinate * wp = new GridCoordinate[MAX_WAYPOINTS];
			int numWp = MAX_WAYPOINTS;
			generateWaypoints(wp, numWp);

			// Recusively smooth path
			int numWpSmooth = numWp;
			GridCoordinate * wpSmooth = new GridCoordinate[numWpSmooth];
			while (smooth(wp, numWp, wpSmooth, numWpSmooth)) {
				numWp = numWpSmooth;
				GridCoordinate * p = wp;
				wp = wpSmooth;
				wpSmooth = p;
			}

			// Save way points
			numWaypoints = 0;
			for (int i = 0; i < numWpSmooth; i ++) {
				wayPoints[numWaypoints ++] = wpSmooth[i];
				Logger::debug("WaveFront", wpSmooth[i].toString());
			}

			delete[] wp;
			delete[] wpSmooth;

			return true;
		}

		GridCoordinate WaveFront::getNextWP(GridCoordinate current) {
			char nextValue = pMap->getValue(current) - 1;
			int x = current.x, y = current.y;

			// Order the list to try to go straight line
			GridCoordinate nodeList[] = {
				GridCoordinate(x-1, y), GridCoordinate(x+1, y),
				GridCoordinate(x, y-1), GridCoordinate(x, y+1), 
				GridCoordinate(x+1, y+1), GridCoordinate(x-1, y+1),
				GridCoordinate(x+1, y-1), GridCoordinate(x-1, y-1)
			};

			int minObstacle = 8;
			GridCoordinate selected;
			for (int i = 0; i < 8; i ++) {
				if (pMap->getValue(nodeList[i]) == nextValue) {
					int density = pMap->getObstacleDensity(nodeList[i]);
					if (density < minObstacle) { // select a node with less obstacles nearby
						minObstacle = density;
						selected = nodeList[i];
					}
				}
			}

			return selected;
		}

		bool WaveFront::smooth(GridCoordinate * wp, int num, GridCoordinate * wpSmooth, int& numSmooth) {
			numSmooth = 0;
			wpSmooth[numSmooth ++] = wp[0];

			int index;
			for (index = 1; index < num - 1; index ++) {
				GridCoordinate wp1 = wp[index - 1];
				GridCoordinate wp2 = wp[index];
				GridCoordinate wp3 = wp[index + 1];

				if (hasObstacle(wp1, wp3)) {
					wpSmooth[numSmooth ++] = wp2;
				} else {
					index ++;
					break;
				}
			}

			for (int i = index; i < num; i ++) {
				wpSmooth[numSmooth ++] = wp[i];
			}

			return num > numSmooth;
		}

		// bool WaveFront::smooth(GridCoordinate * wp, int num, GridCoordinate * wpSmooth, int& numSmooth) {
		// 	numSmooth = 0;
		// 	wpSmooth[numSmooth ++] = wp[0];

		// 	int index;
		// 	for (index = 1; index < num - 1; index += 2) {
		// 		GridCoordinate wp1 = wp[index - 1];
		// 		GridCoordinate wp2 = wp[index];
		// 		GridCoordinate wp3 = wp[index + 1];

		// 		if (hasObstacle(wp1, wp3)) {
		// 			wpSmooth[numSmooth ++] = wp2;
		// 		}
		// 		wpSmooth[numSmooth ++] = wp3;
		// 	}

		// 	if (index != num - 2)
		// 		wpSmooth[numSmooth ++] = wp[num - 1];

		// 	return num > numSmooth;
		// }

		bool WaveFront::hasObstacle(GridCoordinate wp1, GridCoordinate wp2) {
			if (bSkipSmooth) return true;

			int x1 = wp1.x, y1 = wp1.y;
			int x2 = wp2.x, y2 = wp2.y;
			int dx = x2 - x1, dy = y2 - y1;

			if (x1 == x2) {
				if (y1 > y2) {
					y1 += y2; y2 = y1 - y2; y1 = y1 - y2; // swap y1 and y2
				}

				for (int y = y1 + 1; y < y2 - 1; y ++) {
					if (pMap->getValue(GridCoordinate(x1, y)) == 1) return true;
				}
				return false;
			}

			if (y1 == y2) {
				if (x1 > x2) {
					x1 += x2; x2 = x1 - x2; x1 = x1 - x2; // swap x1 and x2
				}

				for (int x = x1 + 1; x < x2 - 1; x ++) {
					if (pMap->getValue(GridCoordinate(x, y1)) == 1) return true;
				}
				return false;
			}

			if (std::abs(dx) == std::abs(dy)) {
				int stepX = dx / std::abs(dx);
				int stepY = dy / std::abs(dy);
				GridCoordinate node(x1 + stepX, y1 + stepY);
				while (!(node == wp2)) {
					if (pMap->getValue(node) == 1) return true;
					node.x += stepX;
					node.y += stepY;
				}
				return false;
			}

			if (std::abs(dx) == 1) {
				if (y1 > y2) {
					x1 += x2; x2 = x1 - x2; x1 = x1 - x2; // swap x1 and x2
					y1 += y2; y2 = y1 - y2; y1 = y1 - y2; // swap y1 and y2
				}

				for (int y = y1 + 1; y <= y2 - 1; y ++) {
					if (pMap->getValue(GridCoordinate(x1, y)) == 1) return true;
					if (pMap->getValue(GridCoordinate(x2, y)) == 1) return true;
				}
				return false;
			}

			if (std::abs(dy) == 1) {
				if (x1 > x2) {
					x1 += x2; x2 = x1 - x2; x1 = x1 - x2; // swap x1 and x2
					y1 += y2; y2 = y1 - y2; y1 = y1 - y2; // swap y1 and y2
				}

				for (int x = x1 + 1; x <= x2 - 1; x ++) {
					if (pMap->getValue(GridCoordinate(x, y1)) == 1) return true;
					if (pMap->getValue(GridCoordinate(x, y2)) == 1) return true;
				}
				return false;
			}

			return true;
		}

		void WaveFront::markPath() {
			for (int i = 0; i < numWaypoints; i ++) {
				pMap->setValue(wayPoints[i], -20);
			}
		}

		void WaveFront::skipSmooth(bool skip) {
			bSkipSmooth = skip;
		}

}