#include "Behavior.h"
#include "PFields.h"
#include "Logger.h"
#include <cmath>

namespace Robot {

	const double MOTOR_MAX_SPEED = 1;
	const double MOTOR_MAX_TRUNRATE = 1;

	Pose Behavior::gotoTarget(PerceptualSchema * ps) {
    double minAttractiveDistance = 2;
    double gotoGain = 1;
    Pose targetPos = ps->getTargetPos();
    Pose gotoResult = PotentialFields::attractive(gotoGain, minAttractiveDistance, targetPos);

    return gotoResult;
	}

  Pose Behavior::avoidObstacle(PerceptualSchema * ps, double maxDistance) {
    double weights[] = {1, 1, 1, 1, 1, 1, 1, 1};
    RangerReading rr = ps->getRangerReading();
    int tangentialDirection = (ps->closeToSide() == PerceptualSchema::CLOSE_TO_RIGHT)? 1: -1;
    // int tangentialDirection = 1;

    Pose avoidResult, normResult;
    for (int i = 0; i < rr.size(); i ++) {
      Pose obstaclePos = rr.get(i);
      double avoidGain = weights[i];
      Pose action = PotentialFields::tangential(avoidGain, maxDistance, tangentialDirection, obstaclePos);
      avoidResult = avoidResult + action;

      Pose normValue = rr.getNormValue(i);
      normValue.speed *= avoidGain;
      normResult = normResult + normValue;
    }
    avoidResult.speed = avoidResult.speed / normResult.speed;

    return avoidResult;
  }

	Pose Behavior::smoothMove(Pose action) {
		Pose result;

		double angleGoal = Angle::normalize(action.yaw);

		result.yaw = std::min(angleGoal, MOTOR_MAX_TRUNRATE);
		result.yaw = std::max(result.yaw, -MOTOR_MAX_TRUNRATE);
		if (std::fabs(angleGoal) < PI/6) {
			result.speed = MOTOR_MAX_SPEED * action.speed;
		}

		return result;

	}
	
}