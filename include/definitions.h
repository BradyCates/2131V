#include "pros/apix.h"
#include "pros/imu.hpp"

bool in_autonomous;
int DRIVING_SPEED = 127;
int storage_control = 0;
int intake_control = 0;
int autonomous_mode = 1;
int driving_mode = 0;
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup DrivetrainL({-5, -3});    
pros::MotorGroup DrivetrainR({8, 10}); 
pros::Motor DrivetrainLF(-4);
pros::Motor Intake(15);
pros::Motor Storage(12);
pros::ADIDigitalOut Arm ('H');
pros::IMU imu(2);
