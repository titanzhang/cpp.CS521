#ifndef __ACT_H__
#define __ACT_H__

#include <libplayerc++/playerc++.h>
#include "Primitives.h"
#include "PerceptualSchema.h"

namespace Robot {

	class Act {
	public:
		Act(PlayerCc::Position2dProxy * pPP, PlayerCc::RangerProxy * pRP);
		void setTarget(Coordinate target);
		Coordinate getTarget();
		bool reachTarget();
		Pose go();
	private:
		PerceptualSchema perceptualSchma;
		Coordinate mTarget;
	};
}

#endif
