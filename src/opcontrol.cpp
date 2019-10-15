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
/*
// motor ports
namespace ports {
    const int DRIVE_LEFT = 1;
    const int DRIVE_RIGHT = 2;
    const int LIFT_LEFT = 3;
    const int LIFT_RIGHT = 4;
};

namespace motors {
    okapi::Motor driveLeft(ports::DRIVE_LEFT);
    okapi::Motor driveRight(ports::DRIVE_RIGHT);
    okapi::Motor liftLeft(ports::LIFT_LEFT);
    okapi::Motor liftRight(ports::LIFT_RIGHT);

    // motor groups:
    okapi::MotorGroup liftGroup({liftLeft, liftRight});
};

// buttons
namespace btn {
	using okapi::ControllerButton;
	using okapi::ControllerDigital;

	// buttons for controlling lift
	ControllerButton liftUp(ControllerDigital::L1);
	ControllerButton liftDown(ControllerDigital::L2);
	ControllerButton liftReset(ControllerDigital::X);

	// button for autonomous
	ControllerButton autoRun(ControllerDigital::A);

	// buttons for driving
	ControllerButton driveForward(ControllerDigital::up);
	ControllerButton driveBackward(ControllerDigital::down);
}

void opcontrol() {
	// drive controls
	auto drive = okapi::ChassisControllerFactory::create(
		motors::driveLeft, motors::driveRight
	);

	// joystick input
	okapi::Controller controller;

	// count of update cycles
	int count = 0;

	while (true) {
		count++;
		
		// drive tank controls
		{
			using okapi::ControllerAnalog;
			drive.tank(controller.getAnalog(ControllerAnalog::leftY),
					controller.getAnalog(ControllerAnalog::rightY));
		}

		// autonomous
		if (btn::autoRun.isPressed()) {
			autonomous();
		}

		// logic for controlling lift with buttons
		// manual control
		if (btn::liftUp.isPressed()) {
			motors::liftGroup.moveVoltage(12000);
		} else if (btn::liftDown.isPressed()) {
			motors::liftGroup.moveVoltage(-3000);
		} else if (btn::liftReset.isPressed()) {
			motors::liftGroup.moveAbsolute(0, 60);
		} else {
			motors::liftGroup.moveVoltage(0);
		}

		// move forward/backward
		if (btn::driveForward.isPressed()) {
			// TODO
			drive.forward(0.5);
		} else if (btn::driveBackward.isPressed()) {
			drive.forward(-0.5);
		}

		pros::delay(10);
	}
}
*/

okapi::Motor motor(1);

void opcontrol() {
	motor.move_voltage(12000);
}