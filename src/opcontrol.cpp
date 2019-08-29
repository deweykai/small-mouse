#include "main.h"
#include <string>

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
const int DRIVE_MOTOR_RIGHT = 1;
const int DRIVE_MOTOR_LEFT = 2;
const int LIFT_MOTOR_RIGHT = 3;
const int LIFT_MOTOR_LEFT = 4;
const int MIDDLE_MOTOR = 5;

// motors
Motor driveLeftMotor(DRIVE_MOTOR_LEFT);
Motor driveRightMotor(-DRIVE_MOTOR_RIGHT);
Motor liftLeftMotor(-LIFT_MOTOR_LEFT);
Motor liftRightMotor(LIFT_MOTOR_RIGHT);
pros::Motor middleMotor(MIDDLE_MOTOR);

// motor groups
const MotorGroup liftMotorGroup({liftLeftMotor, liftRightMotor});

// dimensions used for tracking position
const auto WHEEL_DIAMETER = 10_cm;
const auto CHASSIS_WIDTH = 37_cm;

auto drive = ChassisControllerFactory::create(
	DRIVE_MOTOR_LEFT, -DRIVE_MOTOR_RIGHT,
	AbstractMotor::gearset::green,
	{WHEEL_DIAMETER, CHASSIS_WIDTH}
);

// control of lift motors async from main loop
auto liftControl = AsyncControllerFactory::posIntegrated(liftMotorGroup);

// predefined heights for lift
const int NUM_HEIGHTS = 4;
const int height0 = 0;
const int height1 = 20;
const int height2 = 40;
const int height3 = 90;
const int heights[NUM_HEIGHTS] = {height0, height1, height2, height3};
const int armGearRatio = 7;

// buttons for controlling lift
ControllerButton btnUp(ControllerDigital::up);
ControllerButton btnDown(ControllerDigital::down);
ControllerButton driveMode(ControllerDigital::R1);

void opcontrol() {
	// joystick input
	// defaults to master
	Controller controller;
	pros::Controller master(CONTROLLER_MASTER);

	// value to track the current goal height
	int goalHeight = 0;

	// count of update cycles
	int count = 0;

	// lift motor test:
	if (false) {
		pros::Motor leftMotor (LIFT_MOTOR_LEFT);
		pros::Motor rightMotor (LIFT_MOTOR_RIGHT);
		leftMotor.move(50);
		rightMotor.move(50);
		pros::delay(1000);
		leftMotor.move(0);
		rightMotor.move(0);
	}

	// set max angular velocity for arms
	int maxAngularVelocity = 80;
	liftControl.setMaxVelocity(maxAngularVelocity);

	while (true) {
		// print to lcd screen
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		// drive tank controls
		drive.tank(controller.getAnalog(ControllerAnalog::leftY),
				controller.getAnalog(ControllerAnalog::rightY));

		// move the middle motor
		int middleMotorPower = 0;
		int turbo = master.get_digital(DIGITAL_X);
		if (master.get_digital(DIGITAL_LEFT)) {
			middleMotorPower = -50;
		} else if (master.get_digital(DIGITAL_RIGHT)) {
			middleMotorPower = 50;
		}
		if (turbo) {
			middleMotorPower *= 2;
		}
		middleMotor.move(middleMotorPower);

		// logic for controlling lift with buttons
		// set the target height
		if (btnUp.changedToPressed() && goalHeight < NUM_HEIGHTS - 1) {
			goalHeight++;
			liftControl.setTarget(heights[goalHeight] * armGearRatio);
		} else if (btnDown.changedToPressed() && goalHeight > 0) {
			goalHeight--;
			liftControl.setTarget(heights[goalHeight] * armGearRatio);
		}

		// controller lcd
		// issue: it don't work
		if (!(count % 100)) {
			controller.setText(0, 0, "height: " + heights[goalHeight]);
		}

		pros::delay(10);
	}
}
