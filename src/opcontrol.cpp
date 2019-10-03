#include "main.h"
#include "motors.h"
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

// buttons
using okapi::ControllerButton;
using okapi::ControllerDigital;

// buttons for controlling lift
ControllerButton btnUp(ControllerDigital::L1);
ControllerButton btnDown(ControllerDigital::L2);
ControllerButton resetLift(ControllerDigital::down);

// button for autonomous
ControllerButton btnAuto(ControllerDigital::A);

void opcontrol() {
	// drive controls
	auto drive = okapi::ChassisControllerFactory::create(
		ports::DRIVE_MOTOR_LEFT, -ports::DRIVE_MOTOR_RIGHT
	);

	// joystick input
	okapi::Controller controller;

	// defaults to master
	pros::Controller master(CONTROLLER_MASTER);

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
		if (btnAuto.isPressed()) {
			autonomous();
		}

		// logic for controlling lift with buttons
		// manual control
		if (btnUp.isPressed()) {
			motors::liftGroup.moveVoltage(12000);
		} else if (btnDown.isPressed()) {
			motors::liftGroup.moveVoltage(-3000);
		} else if (resetLift.isPressed()) {
			motors::liftGroup.moveAbsolute(0, 60);
		} else {
			motors::liftGroup.moveVoltage(1000);
		}

		// move forward/backward
		if (master.get_digital(DIGITAL_UP)) {
			// TODO
			drive.forward(0.5);
		} else if (master.get_digital(DIGITAL_DOWN)) {
			drive.forward(-0.5);
		}

		pros::delay(10);
	}
}
