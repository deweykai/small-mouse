#include "main.h"
#include "okapi/api.hpp"

using namespace okapi;

const auto WHEEL_DIAMETER = 10_cm;
const auto CHASSIS_WIDTH = 37_cm;

// motor ports
namespace ports {
    const int DRIVE_LEFT = 1;
    const int DRIVE_RIGHT = 2;
//    const int LIFT_LEFT = 3;
//    const int LIFT_RIGHT = 4;
//    const int INTAKE = 5;
	const int CENTER = 10;
};

namespace motors {
    Motor drive_left(ports::DRIVE_LEFT);
    Motor drive_right(ports::DRIVE_RIGHT);

	Motor center(ports::CENTER);

	void init() {
    }
};

auto drive = ChassisControllerFactory::create(
	motors::drive_left, motors::drive_right,
	AbstractMotor::gearset::green,
	{WHEEL_DIAMETER, CHASSIS_WIDTH}
);

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
	motors::init();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

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
void autonomous() {
	motors::drive_left.move_voltage(12000);
}

void debug() {
	static int count = 0;
	count = (count + 1) % 200;
	if (count != 0) {
		return;
	}

	int current_draw = motors::drive_left.get_current_draw();
	printf("motor: %d\n", current_draw);
}

namespace btn {
	ControllerButton left (ControllerDigital::left);
	ControllerButton right (ControllerDigital::right);
}

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
void opcontrol() {
	Controller master;

	while (true) {
		debug();

		// tank controls
		drive.tank(
			master.getAnalog(ControllerAnalog::leftY),
			master.getAnalog(ControllerAnalog::rightY)
		);

		if (btn::left.isPressed()) {
			motors::center.move_voltage(12000);
		} else if (btn::right.isPressed()) {
			motors::center.move_voltage(-12000);
		} else {
			motors::center.move_voltage(0);
		}

		pros::delay(10);
	}
}