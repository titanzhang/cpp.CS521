#include "Act.h"
#include <cmath>

namespace Robot {

	const double MOTOR_MAX_TURNRATE = 1;
	const double MOTOR_MAX_SPEED = 0.5;

	Act::Act(PlayerCc::Position2dProxy * pPP): mPositionProxy(pPP) {}

	// Just make sure speed/yaw is within physical limitation
	void Act::go(Pose p) {
		double speed = std::min(p.speed, MOTOR_MAX_SPEED);
		double yaw = std::min(p.yaw, MOTOR_MAX_TURNRATE);
    yaw = std::max(yaw, -MOTOR_MAX_TURNRATE);

    mPositionProxy->SetSpeed(speed, yaw);
	}

	void Act::stop() {
		Pose p(0,0);
		this->go(p);
	}
}