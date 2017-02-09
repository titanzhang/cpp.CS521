#include <libplayerc++/playerc++.h>
#include <iostream>
#include "Primitives.h"
#include "Logger.h"
#include "PerceptualSchema.h"
#include "Behavior.h"
#include "PFields.h"
#include <thread>
#include <chrono>
#include <cstdlib>

using namespace Robot;

int main(int argc, char **argv) {
  try {
    // Parse command line parameters
    if (argc < 3) {
      throw RobotException("Please specify target");
    }

    Coordinate target(atof(argv[1]), atof(argv[2]));
    Logger::info("Target", target.toString());
    // Coordinate target(7, 3);

    // Initialize player/stage
    PlayerCc::PlayerClient robot(PlayerCc::PLAYER_HOSTNAME, PlayerCc::PLAYER_PORTNUM);
    PlayerCc::Position2dProxy pp(&robot, 0);
    PlayerCc::RangerProxy rp (&robot, 0);
    pp.SetMotorEnable (true);

    //Initialize robot modules
    PerceptualSchema perceptualSchma(&pp, &rp, target);

    // Main Loop
    while (true) {
      robot.Read();

      if (perceptualSchma.reachTarget()) {
        std::cout << "Target reached!" << std::endl;
        break;
      }

      Pose gotoResult, avoidResult, combinedResult;
      double avoidDistance = 1.2;
      double dangerousDistance = 0.5;

      // Releasers
      bool avoid = perceptualSchma.nearObstacle(avoidDistance);
      bool danger = perceptualSchma.nearObstacle(dangerousDistance);
      bool closeToTarget = perceptualSchma.nearTarget();

      if (avoid && !closeToTarget) { // Near obstacles, activate avoid behavior
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

      if (combinedResult.speed == 0) {
        combinedResult = PotentialFields::noisy(0.1);
      }

      // Execute action
      combinedResult = Behavior::smoothMove(combinedResult);
      pp.SetSpeed(combinedResult.speed, combinedResult.yaw);

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

