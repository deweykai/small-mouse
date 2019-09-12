#ifndef MOTOR_H
#define MOTOR_H

#include "main.h"
using namespace okapi;

// motor ports
const int DRIVE_MOTOR_RIGHT = 1;
const int DRIVE_MOTOR_LEFT = 2;
const int LIFT_MOTOR_RIGHT = 3;
const int LIFT_MOTOR_LEFT = 4;
const int LIFT_MOTOR_RIGHT_2 = 5;
const int LEFT_MOTOR_LEFT_2 = 6;
const int MIDDLE_MOTOR = 7;
const int CLAW_MOTOR = 8;

// motors
Motor driveLeftMotor(DRIVE_MOTOR_LEFT);
Motor driveRightMotor(-DRIVE_MOTOR_RIGHT);
Motor liftLeftMotor(-LIFT_MOTOR_LEFT);
Motor liftLeftLowerMotor(-LIFT_MOTOR_LEFT);
Motor liftRightMotor(LIFT_MOTOR_RIGHT);
Motor liftRightLowerMotor(LIFT_MOTOR_RIGHT);
Motor middleMotor(MIDDLE_MOTOR);
Motor clawMotor(-CLAW_MOTOR);

// motor groups
MotorGroup liftMotorGroup({liftLeftMotor, liftRightMotor, liftLeftLowerMotor, liftRightLowerMotor});

// dimensions used for tracking position
const auto WHEEL_DIAMETER = 10_cm;
const auto CHASSIS_WIDTH = 37_cm;

auto drive = ChassisControllerFactory::create(
	DRIVE_MOTOR_LEFT, -DRIVE_MOTOR_RIGHT,
	AbstractMotor::gearset::green,
	{WHEEL_DIAMETER, CHASSIS_WIDTH}
);

#endif