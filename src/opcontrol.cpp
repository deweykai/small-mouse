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
// buttons for controlling lift
using okapi::ControllerButton;
using okapi::ControllerDigital;
ControllerButton btnUp(ControllerDigital::L1);
ControllerButton btnDown(ControllerDigital::L2);
ControllerButton resetLift(ControllerDigital::down);
// buttons for claw
ControllerButton btnOpen(ControllerDigital::R2);
ControllerButton btnClose(ControllerDigital::R1);
// button for autonomous
ControllerButton btnAuto(ControllerDigital::A);

void opcontrol() {
	// drive controls
	auto drive = okapi::ChassisControllerFactory::create(
		DRIVE_MOTOR_LEFT, -DRIVE_MOTOR_RIGHT
	);

	// joystick input
	// defaults to master
	okapi::Controller controller;
	pros::Controller master(CONTROLLER_MASTER);

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

	while (true) {
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
		middleMotor.moveVelocity(middleMotorPower);

		// logic for controlling lift with buttons
		// manual control
		if (btnUp.isPressed()) {
			liftMotorGroup.moveVoltage(12000);
		} else if (btnDown.isPressed()) {
			liftMotorGroup.moveVoltage(-3000);
		} else if (resetLift.isPressed()) {
			liftMotorGroup.moveAbsolute(0, 60);
		} else {
			liftMotorGroup.moveVoltage(1000);
		}

		// move forward/backward
		if (master.get_digital(DIGITAL_UP)) {
			// TODO
			drive.forward(0.5);
		} else if (master.get_digital(DIGITAL_DOWN)) {
			drive.forward(-0.5);
		}

		// control for the claw motor
		if (btnClose.isPressed()) {
			clawMotor.moveVoltage(12000);
		} else if (btnOpen.isPressed()) {
			clawMotor.moveVoltage(-3000);
		} else {
			clawMotor.moveVoltage(0);
		}

    master.clear_line(0);
    master.print(0, 0, "Hello");

		pros::delay(10);
	}
}
