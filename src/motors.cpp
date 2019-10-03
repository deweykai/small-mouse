#include "motors.h"

using namespace okapi;

// motors
namespace motors {
    Motor driveLeft(ports::DRIVE_MOTOR_LEFT);
    Motor driveRight(-ports::DRIVE_MOTOR_RIGHT);
    Motor liftLeft(ports::LIFT_MOTOR_LEFT);
    Motor liftRight(ports::LIFT_MOTOR_RIGHT);

    // motor groups
    MotorGroup liftGroup({liftLeft, liftRight});

    // init function:
    void init() {
        liftGroup.setEncoderUnits(Motor::encoderUnits::rotations);
    }
}