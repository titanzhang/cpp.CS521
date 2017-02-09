#include "Pilot.h"
#include <cmath>
#include "Logger.h"

namespace Robot {

	const double PILOT_FINISH_DISTANCE = 0.02;
	const double PILOT_MAX_SPEED = 0.5;
	const double PILOT_MAX_TRUNRATE = 1;

	Pilot::Pilot(PlayerCc::Position2dProxy * pPP): mPositionProxy(pPP) { }

	bool Pilot::isTargetReach() {
		double current_x = mPositionProxy->GetXPos();
		double current_y = mPositionProxy->GetYPos();
		double goalDistance = mTarget.distanceTo(current_x, current_y);

		char buf[100];
		snprintf(buf, 100, "current=[ %.3f %.3f ]; distance=%.3f", current_x, current_y, goalDistance);
		Logger::debug("Pilot.isTargetReach", buf);

		if (goalDistance < PILOT_FINISH_DISTANCE) {
			Logger::info("Pilot[reached]", buf);
			return true;
		} else {
			return false;
		}
	}

	void Pilot::setTarget(Coordinate wayPoint) {
		mTarget = wayPoint;

		Logger::debug("Pilot.setTarget", mTarget.toString());
		Logger::info("Pilot[target]", mTarget.toString());
	}

	Pose Pilot::getAction() {
		Pose action;

		Coordinate currentPosition(mPositionProxy->GetXPos(), mPositionProxy->GetYPos());
		double angleGoal = Angle::normalize(currentPosition.angle(mTarget) - mPositionProxy->GetYaw());
		double distancdGoal = currentPosition.distanceTo(mTarget);

		action.yaw = std::min(angleGoal, PILOT_MAX_TRUNRATE);
		action.yaw = std::max(action.yaw, -PILOT_MAX_TRUNRATE);
		if (std::fabs(angleGoal) < PI/3) {
			action.speed = std::min(PILOT_MAX_SPEED, distancdGoal);
		}

		Logger::debug("Pilot.getAction", action.toString());

		return action;
	}
	
}