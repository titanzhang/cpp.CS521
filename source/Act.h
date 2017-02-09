#ifndef __ACT_H__
#define __ACT_H__

#include <libplayerc++/playerc++.h>
#include "Primitives.h"

namespace Robot {

	extern const double MOTOR_MAX_TURNRATE;
	extern const double MOTOR_MAX_SPEED;

	class Act {
	public:
		Act(PlayerCc::Position2dProxy * pPP);
		void go(Pose p);
		void stop();
	private:
		PlayerCc::Position2dProxy* mPositionProxy;
	};
}

#endif
