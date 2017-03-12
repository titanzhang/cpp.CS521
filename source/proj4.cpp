#include <libplayerc++/playerc++.h>
#include <iostream>
#include "Primitives.h"
#include "Logger.h"
#include "Navigator.h"
#include "Pilot.h"
#include "Act.h"
#include <thread>
#include <chrono>
#include <cstdlib>

using namespace Robot;

std::string getDirectory(std::string execPath) {
  int find = execPath.find_last_of('/');
  return (find == std::string::npos)? "": execPath.substr(0, find+1);
}

int main(int argc, char **argv) {
  try {
    // Parse command line parameters
    if (argc < 3) {
      throw RobotException("Please specify target");
    }

    std::string mapFile = getDirectory(argv[0]) + "hospital_section.pnm";
    Coordinate target(atof(argv[1]), atof(argv[2]));
    Logger::info("Target", target.toString());

    // Initialize player/stage
    PlayerCc::PlayerClient robot(PlayerCc::PLAYER_HOSTNAME, PlayerCc::PLAYER_PORTNUM);
    PlayerCc::Position2dProxy pp(&robot, 0);
    PlayerCc::RangerProxy rp (&robot, 0);
    pp.SetMotorEnable (true);

    //Initialize robot modules
    // Navigator
    Navigator navigator;
    navigator.readMap(mapFile, 0.0368, 0.0406, 10);
    // navigator.readMap(mapFile, 0.04, 0.04, 10);
    robot.Read();
    Coordinate current(pp.GetXPos(), pp.GetYPos());
    Logger::debug("pp", current.toString());
    if (!navigator.planPath(current, target)) {
      throw RobotException("Navigator: Can not find a path!");
    }
    // Pilot
    int size;
    Coordinate * wayPoints = navigator.getWaypoints(&size);
    Pilot pilot(wayPoints, size);
    // Act
    Act act(&pp, &rp);
    if (!pilot.hasNext()) {
      throw RobotException("Pilot: No more way points!");
    }
    act.setTarget(pilot.next());
    Logger::info("Act", act.getTarget().toString());

    // Main Loop
    while (true) {
      robot.Read();

      if (act.reachTarget()) {
        if (!pilot.hasNext()) break;
        act.setTarget(pilot.next());
        Logger::info("Act", act.getTarget().toString());
      }

      Pose action = act.go();
      pp.SetSpeed(action.speed, action.yaw);

      // std::chrono::seconds sec(2);
      // std::this_thread::sleep_for(sec);
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
