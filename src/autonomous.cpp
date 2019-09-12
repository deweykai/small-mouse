#include "main.h"
#include "motors.h"
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

// dimensions used for tracking position
const auto WHEEL_DIAMETER = 10_cm;
const auto CHASSIS_WIDTH = 37_cm;

void autonomous() {
    auto drive = ChassisControllerFactory::create(
        DRIVE_MOTOR_LEFT, -DRIVE_MOTOR_RIGHT,
        AbstractMotor::gearset::green,
        {WHEEL_DIAMETER, CHASSIS_WIDTH}
    );
    
    drive.moveDistance(10);
}