#ifndef MOTOR_H
#define MOTOR_H

#include "main.h"

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
extern okapi::Motor driveLeftMotor;
extern okapi::Motor driveRightMotor;
extern okapi::Motor liftLeftMotor;
extern okapi::Motor liftLeftLowerMotor;
extern okapi::Motor liftRightMotor;
extern okapi::Motor liftRightLowerMotor;
extern okapi::Motor middleMotor;
extern okapi::Motor clawMotor;

// motor groups:
extern okapi::MotorGroup liftMotorGroup;

#endif