#include "main.h"

/****************
 * motors setup
 * **************/
namespace ports
{
const int DRIVE_LEFT = 1;
const int DRIVE_RIGHT = 2;
const int LIFT_LOW_LEFT = 11;
const int LIFT_LOW_RIGHT = 12;
const int LIFT_HIGH_LEFT = 13;
const int LIFT_HIGH_RIGHT = 14;
const int INTAKE = 7;
const int CENTER = 10;
}; // namespace ports

namespace motors
{
// drive motors
Motor drive_left(ports::DRIVE_LEFT);
Motor drive_right(-ports::DRIVE_RIGHT);

// lift motors
Motor lift_low_left(ports::LIFT_LOW_LEFT);
Motor lift_low_right(-ports::LIFT_LOW_RIGHT);
Motor lift_high_left(-ports::LIFT_HIGH_LEFT);
Motor lift_high_right(ports::LIFT_HIGH_RIGHT);

// intake motor
Motor intake(ports::INTAKE);

// center motor
Motor center(-ports::CENTER);

MotorGroup lift_group({lift_low_left, lift_low_right, lift_high_left, lift_high_right});

void init()
{
	lift_group.setBrakeMode(Motor::brakeMode::brake);
	intake.setGearing(Motor::gearset::red);
	intake.setBrakeMode(Motor::brakeMode::hold);
}
}; // namespace motors

const auto WHEEL_DIAMETER = 10_cm;
const auto CHASSIS_WIDTH = 38_cm;

auto drive = okapi::ChassisControllerFactory::create(
	motors::drive_left, motors::drive_right,
	okapi::AbstractMotor::gearset::green,
	{WHEEL_DIAMETER, CHASSIS_WIDTH});

/**************
 * setup code
 **************/
void debug(void *param)
{
	while (true)
	{
		int current_draw = motors::drive_left.get_current_draw();
		printf("motor: %d\n", current_draw);
		pros::delay(500);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
	// needs the delay to work on a normal orientation.
	pros::delay(10);
	mouse_display::start_display();
	motors::init();
	//pros::Task debug_task(debug, NULL, "DEBUG");
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

/****************
 * control code
 * **************/

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
void autonomous()
{
	motors::drive_left.move_voltage(12000);
}

namespace btn
{
// strafing
ControllerButton left(ControllerDigital::left);
ControllerButton right(ControllerDigital::right);

// lift
ControllerButton lift_up(ControllerDigital::L1);
ControllerButton lift_down(ControllerDigital::L2);

// intake
ControllerButton intake_in(ControllerDigital::R1);
ControllerButton intake_out(ControllerDigital::R2);
ControllerButton stack(ControllerDigital::B);

// test autonomous
ControllerButton auto_test(ControllerDigital::A);
} // namespace btn

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
void opcontrol()
{
	Controller master;

	// the max height in terms of rotations
	// uses a gear ratio of 1:25
	int max_height = 2700;

	// freeze position
	bool frozen = false;
	int position = 0;

	while (true)
	{
		/**** DRIVE ****/
		drive.tank(
			master.getAnalog(ControllerAnalog::leftY),
			master.getAnalog(ControllerAnalog::rightY));

		/**** AUTONOMOUS ****/
		if (btn::auto_test.isPressed())
		{
			autonomous();
		}

		/**** MIDDLE MOTOR ****/
		if (btn::left.isPressed())
		{
			motors::center.move_voltage(12000);
		}
		else if (btn::right.isPressed())
		{
			motors::center.move_voltage(-12000);
		}
		else
		{
			motors::center.move_voltage(0);
		}

		/**** LIFT CONTROLS ****/
		if (btn::lift_up.isPressed())
		{
			motors::lift_group.moveAbsolute(max_height, 200);
			frozen = false;
		}
		else if (btn::lift_down.isPressed())
		{
			motors::lift_group.moveAbsolute(0, 200);
			frozen = false;
		}
		else
		{
			if (!frozen)
			{
				frozen = true;
				position = motors::lift_group.getPosition();
			}
			motors::lift_group.moveVoltage(0);
		}

		/**** INTAKE ****/
		if (btn::intake_in.isPressed())
		{
			motors::intake.moveVoltage(12000);
		}
		else if (btn::intake_out.isPressed())
		{
			motors::intake.moveVoltage(-12000);
		}
		else if (btn::stack.isPressed())
		{
			motors::intake.moveVelocity(-30);
		}
		else
		{
			motors::intake.moveVoltage(0);
		}

		// delay so other tasks can advance
		pros::delay(10);
	}
}
