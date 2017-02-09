#ifndef __PERCEPTUALSCHEMA_H__
#define __PERCEPTUALSCHEMA_H__

#include <libplayerc++/playerc++.h>
#include "Primitives.h"

namespace Robot {
	class PerceptualSchema {
	public:
		static const int CLOSE_TO_LEFT;
		static const int CLOSE_TO_RIGHT;

		PerceptualSchema(PlayerCc::Position2dProxy * pPP, PlayerCc::RangerProxy * pRP, Coordinate goal);
		Coordinate getCurrentCoordiniate();
		double getCurrentDirection();
		Coordinate getGoal();
		RangerReading getRangerReading();
		Pose getTargetPos();
		bool nearObstacle(double maxDistance);
		int closeToSide();
		bool nearTarget();
		bool reachTarget();
		Pose getNearestObstacle(double maxDistance);

	private:
		PlayerCc::Position2dProxy * pPosProxy;
		PlayerCc::RangerProxy * pRangerProxy;
		Coordinate mGoal;
	};
}

#endif
