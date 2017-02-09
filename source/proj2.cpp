#include <libplayerc++/playerc++.h>
#include <iostream>
#include "Primitives.h"
#include "Navigator.h"
#include "Pilot.h"
#include "Act.h"

using namespace Robot;

int main(int argc, char **argv) {

  try {
    // Parse command line parameters
    if (argc < 2) {
      throw RobotException("Please specify waypoint file");
    }
    std::string waypointFileName(argv[1]);

    // Initialize player/stage
    PlayerCc::PlayerClient robot(PlayerCc::PLAYER_HOSTNAME, PlayerCc::PLAYER_PORTNUM);
    PlayerCc::Position2dProxy pp(&robot, 0);
    pp.SetMotorEnable (true);

    //Initialize robot modules
    Navigator navigator;
    navigator.initFromFile(waypointFileName);

    Pilot pilot(&pp);
    if (!navigator.hasNext()) {
      throw RobotException("Empty waypoint list");
    }
    pilot.setTarget(navigator.next());

    Act act(&pp);


    // Main Loop
    while (true) {
      // Update sensory data
      robot.Read();

      // Check if target is reached
      if (pilot.isTargetReach()) {
        if (navigator.hasNext()) {
          pilot.setTarget(navigator.next());
        } else {
          act.stop();
          break;
        }
      }

      // Motion data based on target and current pose
      Pose action = pilot.getAction();

      // Execute action
      act.go(action);
    }
  }
  catch (PlayerCc::PlayerError & e) {
    std::cerr << e << std::endl;
    return -1;
  }
  catch (RobotException & e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  std::cout << "Robot exit!" << std::endl;
}

