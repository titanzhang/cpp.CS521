#include "Act.h"
#include "Behavior.h"
#include "PFields.h"
#include <cmath>

namespace Robot {

	Act::Act(PlayerCc::Position2dProxy * pPP, PlayerCc::RangerProxy * pRP): perceptualSchma(pPP, pRP) {}

	bool Act::reachTarget() {
		return perceptualSchma.reachTarget();
	}

	void Act::setTarget(Coordinate target) {
		mTarget = target;
	}

	Coordinate Act::getTarget() {
		return mTarget;
	}

	Pose Act::go() {
		perceptualSchma.setGoal(mTarget);

    Pose gotoResult, avoidResult, combinedResult;
    double avoidDistance = 0.4;
    double dangerousDistance = 0.2;

    // Releasers
    bool avoid = perceptualSchma.nearObstacle(avoidDistance);
    bool danger = perceptualSchma.nearObstacle(dangerousDistance);
    bool closeToTarget = perceptualSchma.nearTarget();

    if (avoid && !closeToTarget) { // Near obstacles, activate avoid behavior
    // if (avoid) { // Near obstacles, activate avoid behavior
      gotoResult = Behavior::gotoTarget(&perceptualSchma);
      avoidResult = Behavior::avoidObstacle(&perceptualSchma, avoidDistance);

      if (danger) { // Too close to obstacle, stop the goto behavior
        gotoResult.speed *= 0;
        avoidResult.speed *= 1;
      } else {
        gotoResult.speed *= 0.2;
        avoidResult.speed *= 0.8;
      }

      combinedResult = gotoResult + avoidResult;
    } else { // No obstacles around or close to target, go to target!
      gotoResult = Behavior::gotoTarget(&perceptualSchma);
      combinedResult = gotoResult;
    }

    // if (combinedResult.speed == 0) {
    //   combinedResult = PotentialFields::noisy(0.1);
    // }

    // Execute action
    combinedResult = Behavior::smoothMove(combinedResult);

    return combinedResult;
	}

}