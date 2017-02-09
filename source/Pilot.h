#ifndef __PILOT_H__
#define __PILOT_H__

#include <libplayerc++/playerc++.h>
#include "Primitives.h"

namespace Robot {

	extern const double PILOT_FINISH_DISTANCE;
	extern const double PILOT_MAX_SPEED;
	extern const double PILOT_MAX_TRUNRATE;

	class Pilot {
	public:
		Pilot(PlayerCc::Position2dProxy * pPP);
		bool isTargetReach();
		void setTarget(Coordinate wayPoint);
		Pose getAction();

	private:
		PlayerCc::Position2dProxy * mPositionProxy;
		Coordinate mTarget;
	};

}

#endif
