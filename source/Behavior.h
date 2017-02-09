#ifndef __BEHAVIOR_H__
#define __BEHAVIOR_H__

#include "Primitives.h"
#include "PerceptualSchema.h"

namespace Robot {

	extern const double MOTOR_MAX_SPEED;
	extern const double MOTOR_MAX_TRUNRATE;

	class Behavior {
	public:
		static Pose gotoTarget(PerceptualSchema * ps);
		static Pose avoidObstacle(PerceptualSchema * ps, double maxDistance);
		static Pose smoothMove(Pose action);
	};
}

#endif