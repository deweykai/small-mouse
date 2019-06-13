#include "main.h"

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

using namespace okapi;

// motor ports
const Motor DRIVE_MOTOR_RIGHT = 1_rmtr;
const Motor DRIVE_MOTOR_LEFT = 2_mtr;
const Motor LIFT_MOTOR_RIGHT = 3_mtr;
const Motor LIFT_MOTOR_LEFT = 4_rmtr;
const MotorGroup LIFT_MOTOR_GROUP({LIFT_MOTOR_LEFT, LIFT_MOTOR_RIGHT});

// dimensions used for tracking position
const auto WHEEL_DIAMETER = 10_cm;
const auto CHASSIS_WIDTH = 37_cm;

auto drive = ChassisControllerFactory::create(
	DRIVE_MOTOR_LEFT, DRIVE_MOTOR_RIGHT,
	AbstractMotor::gearset::green,
	{WHEEL_DIAMETER, CHASSIS_WIDTH}
);

// control of lift motors async from main loop
auto liftControl = AsyncControllerFactory::posIntegrated(LIFT_MOTOR_GROUP);

// predefined heights for lift
const int NUM_HEIGHTS = 4;
const int height1 = 0;
const int height2 = 20;
const int height3 = 40;
const int height4 = 90;
const int heights[NUM_HEIGHTS] = {height1, height2, height3, height4};
const int armGearRatio = 7;

// buttons for controlling lift
ControllerButton btnUp(ControllerDigital::R1);
ControllerButton btnDown(ControllerDigital::R2);

void opcontrol() {
	// joystick input
	// defaults to master
	Controller controller;

	// value to track the current goal height
	int goalHeight = 0;

	// set max angular velocity for arms
//	int maxAngularVelocity = 1;
//	leftLiftControl.setMaxVelocity(maxAngularVelocity);
//	rightLiftControl.setMaxVelocity(maxAngularVelocity);

	while (true) {
		// print to lcd screen
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		controller.clearLine(1);
		controller.setText(1, 0, "Hello");

		// drive tank controls
		drive.tank(controller.getAnalog(ControllerAnalog::leftY),
				   controller.getAnalog(ControllerAnalog::rightY));

		// logic for controlling lift with buttons
		// set the target height
		if (btnUp.changedToPressed() && goalHeight < NUM_HEIGHTS - 1) {
			goalHeight++;
			liftControl.setTarget(heights[goalHeight] * armGearRatio);
		} else if (btnDown.changedToPressed() && goalHeight > 0) {
			goalHeight--;
			liftControl.setTarget(heights[goalHeight] * armGearRatio);
		}

		pros::delay(20);
	}
}
