#include "motors.h"

using namespace okapi;


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