#include "main.h"
/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
using namespace okapi;

/*
namespace motors {
    okapi::Motor driveLeft(ports::DRIVE_LEFT);
    okapi::Motor driveRight(ports::DRIVE_RIGHT);
    okapi::Motor liftLeft(ports::LIFT_LEFT);
    okapi::Motor liftRight(ports::LIFT_RIGHT);

    // motor groups:
    okapi::MotorGroup liftGroup({liftLeft, liftRight});
};
*/

// dimensions used for tracking position
const auto WHEEL_DIAMETER = 10_cm;
const auto CHASSIS_WIDTH = 37_cm;

void autonomous() {
    /*
    auto drive = ChassisControllerFactory::create(
        motors::driveLeft, motors::driveRight,
        AbstractMotor::gearset::green,
        {WHEEL_DIAMETER, CHASSIS_WIDTH}
    );

    auto profileController = AsyncControllerFactory::motionProfile(
        1.0,  // Maximum linear velocity of the Chassis in m/s
        2.0,  // Maximum linear acceleration of the Chassis in m/s/s
        10.0, // Maximum linear jerk of the Chassis in m/s/s/s
        drive
    );

    profileController.generatePath({
      Point{0_m, 0_m, 0_deg},
      Point{50_cm, 50_cm, 0_deg}},
      "A"
    );

    profileController.setTarget("A");
    profileController.waitUntilSettled();
    */
}
