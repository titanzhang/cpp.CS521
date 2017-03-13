#include "PerceptualSchema.h"

namespace Robot {

	const int PerceptualSchema::CLOSE_TO_LEFT = 1;
	const int PerceptualSchema::CLOSE_TO_RIGHT = 2;

	PerceptualSchema::PerceptualSchema(PlayerCc::Position2dProxy * pPP, PlayerCc::RangerProxy * pRP, Coordinate goal): pPosProxy(pPP), pRangerProxy(pRP), mGoal(goal) { }
	PerceptualSchema::PerceptualSchema(PlayerCc::Position2dProxy * pPP, PlayerCc::RangerProxy * pRP): pPosProxy(pPP), pRangerProxy(pRP) { }

	Coordinate PerceptualSchema::getCurrentCoordiniate() {
		return Coordinate(pPosProxy->GetXPos(), pPosProxy->GetYPos());
	}

	double PerceptualSchema::getCurrentDirection() {
		return pPosProxy->GetYaw();
	}

	Coordinate PerceptualSchema::getGoal() {
		return mGoal;
	}

	void PerceptualSchema::setGoal(Coordinate goal) {
		mGoal = goal;
	}

	RangerReading PerceptualSchema::getRangerReading() {
		RangerReading rr;
		int dataSize = pRangerProxy->GetRangeCount();
		for (int i = 0; i < dataSize; i ++) {
			rr.add(pRangerProxy->GetRange(i));
		}
		return rr;
	}

	Pose PerceptualSchema::getTargetPos() {
		Coordinate myCoord = getCurrentCoordiniate();
		Coordinate targetCoord = getGoal();
		double magnitude = myCoord.distanceTo(targetCoord);
		double direction = Angle::normalize(myCoord.angle(targetCoord) - getCurrentDirection());

		return Pose(direction, magnitude);
	}

	bool PerceptualSchema::nearObstacle(double maxDistance) {
		int dataSize = pRangerProxy->GetRangeCount();
		for (int i = 0; i < dataSize; i ++) {
			if (pRangerProxy->GetRange(i) < maxDistance) {
				return true;
			}
		}
		return false;
	}

	int PerceptualSchema::closeToSide() {
		int dataSize = pRangerProxy->GetRangeCount();
		double distance = 0;
		for (int i = 0; i < dataSize; i ++) {
			if (i < dataSize / 2) {
				distance += pRangerProxy->GetRange(i);
			} else {
				distance -= pRangerProxy->GetRange(i);
			}
		}
		return (distance > 0)? CLOSE_TO_RIGHT: CLOSE_TO_LEFT;
	}

	bool PerceptualSchema::nearTarget() {
		return (getTargetPos().speed < 0.2); 
	}

	bool PerceptualSchema::reachTarget() {
		double reachDistance = 0.08;
		return mGoal.distanceTo(getCurrentCoordiniate()) < reachDistance;
	}

	Pose PerceptualSchema::getNearestObstacle(double maxDistance) {
		RangerReading rr = getRangerReading();
		Pose nearest(0, 500);
		for (int i = 0; i < rr.size(); i ++) {
			Pose reading = rr.get(i);
			if (reading.speed > maxDistance) {
				continue;
			}
			if (nearest.speed > reading.speed) {
				nearest = reading;
			}
		}

		return nearest;
	}
}