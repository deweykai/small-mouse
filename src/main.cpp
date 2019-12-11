#include "main.h"

/****************
 * motors setup
 * **************/
namespace ports
{
const int DRIVE_LEFT = 1;
const int DRIVE_RIGHT = 2;

const int LIFT_LEFT = 11;
const int LIFT_RIGHT = 12;

const int INTAKE_LEFT = 7;
const int INTAKE_RIGHT = 8;
}; // namespace ports

namespace motors
{
// drive motors
Motor drive_left(+ports::DRIVE_LEFT);
Motor drive_right(-ports::DRIVE_RIGHT);

// lift motors
Motor lift_high_left(-ports::LIFT_LEFT);
Motor lift_high_right(+ports::LIFT_RIGHT);

// intake motor
Motor intake_left(-ports::INTAKE_LEFT);
Motor intake_right(+ports::INTAKE_RIGHT);

// motor groups
MotorGroup lift_group({lift_high_left, lift_high_right});
MotorGroup intake_group({intake_left, intake_right});

// setup motors
void init()
{
	/* the brake mode for the lift and intake are set to brake to prevent the
	 * lift and intake from moving when they should be stopped.
	 * The current for the lift_group is limited to 80% to help prevent them
	 * from overheating.
	 */
	lift_group.setBrakeMode(Motor::brakeMode::brake);
	lift_group.setCurrentLimit(lift_group.getCurrentLimit() * 0.8);

	intake_group.setBrakeMode(Motor::brakeMode::hold);
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
	/* The debug should be run as a seperate task.
	 * every 500 ms or 0.5 secs, the debug information will be printed to
	 * the terminal
	 */
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
	pros::Task debug_task(debug, NULL, "DEBUG");
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
	/* This autonomous will move the robot backwards, pushing a block into the
	 * score zone.
	 * Then is will move forward. When the robot stops, the forward momentum will
	 * make the intake fold out
	 */
	drive.moveDistance(-50_cm);
	drive.moveDistance(50_cm);
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

	while (true)
	{
		/* DRIVE
		 * the drive.take method takes two parameters:
		 * movement for the left side
		 * movement for the right side
		 */
		drive.tank(
			master.getAnalog(ControllerAnalog::leftY),
			master.getAnalog(ControllerAnalog::rightY));

		/* AUTONOMOUS
		 * this allows for testing of autonomous
		 * a check is preformed so that autonomous can only be called when
		 * the controller is not connected
		 */
		if (btn::auto_test.isPressed() && !pros::competition::is_connected())
		{
			autonomous();
		}

		/* LIFT CONTROLS
		 * There is a lift_up and lift_down button
		 * if the lift is moving up, it moves towards the specified max height
		 * if the lift is moving down, it moves towards the zero position
		 * if no button is pressed, the lift holds position.
		 */
		if (btn::lift_up.isPressed())
		{
			motors::lift_group.moveAbsolute(max_height, 200);
		}
		else if (btn::lift_down.isPressed())
		{
			motors::lift_group.moveAbsolute(0, 200);
		}
		else
		{
			motors::lift_group.moveVoltage(0);
		}

		/* INTAKE
		 * The intake has three buttons: intake_in, intake_out, stack
		 * the stack button is used to slowly release blocks from the intake.
		 * if no buttons are pressed the intake should hold its position.
		 */
		if (btn::intake_in.isPressed())
		{
			motors::intake_group.moveVoltage(12000);
		}
		else if (btn::intake_out.isPressed())
		{
			motors::intake_group.moveVoltage(-12000);
		}
		else if (btn::stack.isPressed())
		{
			motors::intake_group.moveVelocity(-30);
		}
		else
		{
			motors::intake_group.moveVoltage(0);
		}

		// delay so other tasks can advance
		pros::delay(10);
	}
}
