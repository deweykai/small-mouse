#ifndef MOTOR_H
#define MOTOR_H

#include "main.h"

// motor ports
namespace ports {
    const int DRIVE_MOTOR_LEFT = 1;
    const int DRIVE_MOTOR_RIGHT = 2;
    const int LIFT_MOTOR_LEFT = 3;
    const int LIFT_MOTOR_RIGHT = 4;
}

// motors
namespace motors {
    extern okapi::Motor driveLeft;
    extern okapi::Motor driveRight;
    extern okapi::Motor liftLeft;
    extern okapi::Motor liftRight;

    // motor groups:
    extern okapi::MotorGroup liftGroup;
}

#endif