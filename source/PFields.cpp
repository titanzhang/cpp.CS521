#include "PFields.h"
#include <cmath>
#include <cstdlib>

namespace Robot {
	Pose PotentialFields::repulsive(double gain, double maxDistance, Pose obstaclePos) {
		double direction = Angle::normalize(obstaclePos.yaw - PI);
		double magnitude = 0;
		if (obstaclePos.speed < maxDistance) {
			magnitude = (maxDistance - obstaclePos.speed) / maxDistance;
		}

		return Pose(direction, magnitude * gain);
	}

	Pose PotentialFields::tangential(double gain, double maxDistance, int fieldDirection, Pose obstaclePos) {
		double direction = Angle::normalize(obstaclePos.yaw + PI / 2 * fieldDirection);
		double magnitude = 0;
		if (obstaclePos.speed < maxDistance) {
			magnitude = (maxDistance - obstaclePos.speed) / maxDistance;
		}

		return Pose(direction, magnitude * gain);
	}

	Pose PotentialFields::attractive(double gain, double minDistance, Pose targetPos) {
		double direction = Angle::normalize(targetPos.yaw);
		double magnitude = 1;
		if (targetPos.speed < minDistance) {
			magnitude = targetPos.speed / minDistance;
		}

		return Pose(direction, magnitude * gain);
	}

	Pose PotentialFields::noisy(double gain) {
		double magnitude = gain;
		double direction = Angle::normalize((rand() % 360) * PI / 180);
		return Pose(direction, magnitude);
	}
}
