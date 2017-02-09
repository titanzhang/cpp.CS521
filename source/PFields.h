#ifndef __PFIELDS_H__
#define __PFIELDS_H__

#include "Primitives.h"

namespace Robot {
	class PotentialFields {
	public:
		static Pose repulsive(double gain, double maxDistance, Pose obstaclePos);
		static Pose tangential(double gain, double maxDistance, int fieldDirection, Pose obstaclePos);
		static Pose attractive(double gain, double minDistance, Pose targetPos);
		static Pose noisy(double gain);
	};
}

#endif