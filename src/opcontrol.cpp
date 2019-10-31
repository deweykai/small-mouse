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

// motor ports
namespace ports {
    const int DRIVE_LEFT = 1;
    const int DRIVE_RIGHT = 2;
    const int LIFT_LEFT = 3;
    const int LIFT_RIGHT = 4;
	const int LIFTER = 5;
};

namespace motors {
    okapi::Motor driveLeft(ports::DRIVE_LEFT);
    okapi::Motor driveRight(ports::DRIVE_RIGHT);
    okapi::Motor liftLeft(ports::LIFT_LEFT);
    okapi::Motor liftRight(ports::LIFT_RIGHT);
	okapi::Motor lifter(ports::LIFTER);

    // motor groups:
    okapi::MotorGroup liftGroup({liftLeft, liftRight});

	void init() {
        driveRight.setReversed(true);
        liftRight.setReversed(true);

        liftLeft.setGearing(okapi::Motor::gearset::red);
        liftRight.setGearing(okapi::Motor::gearset::red);
        liftGroup.setBrakeMode(okapi::Motor::brakeMode::hold);
		liftGroup.setCurrentLimit(liftGroup.getCurrentLimit() * 4 / 5);
    }
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
    
    // buttons for intake
    ControllerButton intakeIn(ControllerDigital::R1);
    ControllerButton intakeOut(ControllerDigital::R2);
}

void opcontrol() {
	motors::init();

	// drive controls
	auto drive = okapi::ChassisControllerFactory::create(
		motors::driveLeft, motors::driveRight
	);

	int max_height = 700;

	// joystick input
	okapi::Controller controller;

	// count of update cycles
	int count = 0;

	while (true) {
		count++;
		printf("cycle: %d\n", count);
		
		// drive tank controls
		{
			using okapi::ControllerAnalog;
			drive.tank(controller.getAnalog(ControllerAnalog::leftY),
					controller.getAnalog(ControllerAnalog::rightY));
		}

        // intake control
        if (btn::intakeIn.isPressed()) {
            motors::lifter.moveVoltage(12000);
        } else if (btn::intakeOut.isPressed()) {
            motors::lifter.moveVoltage(-12000);
        } else {
            motors::lifter.moveVoltage(0);
        } 

		// autonomous
		if (btn::autoRun.isPressed()) {
			autonomous();
		}

		// logic for controlling lift with buttons
		// manual control
		if (btn::liftUp.isPressed()) {
            motors::liftGroup.moveAbsolute(max_height, 40);
		} else if (btn::liftDown.isPressed()) {
            motors::liftGroup.moveAbsolute(0, 20);
		} else if (btn::liftReset.isPressed()) {
			// hold to reset
			motors::liftGroup.moveAbsolute(0, 60);
		} else {
            motors::liftGroup.moveVelocity(0);
		}
		printf("%d\n", motors::liftGroup.getVoltage());

		// move forward/backward
		if (btn::driveForward.isPressed()) {
			// TODO
			drive.forward(0.5);
		} else if (btn::driveBackward.isPressed()) {
			drive.forward(-0.5);
		}

		pros::delay(10);
		printf("\n");
	}
}
