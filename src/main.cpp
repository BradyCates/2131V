#include "main.h"
#include "pros/apix.h"
#include "pros/imu.hpp"
#include "constants.h"
#include "definitions.h"
void move_forward(float dist){
	float target = DrivetrainL.get_position() + (dist / (M_PI * WHEEL_DIAMETER_IN)) * 360.0;
	while(DrivetrainL.get_position() <= target){
		DrivetrainL.move(50);
		DrivetrainR.move(50);
		pros::delay(20);
	}
	DrivetrainL.move(0);
	DrivetrainR.move(0);
}

void move_backward(float dist){
	float target = DrivetrainL.get_position() - (dist / (M_PI * WHEEL_DIAMETER_IN)) * 360.0;
	while(DrivetrainL.get_position() >= target){
		DrivetrainL.move(-50);
		DrivetrainR.move(-50);
		pros::delay(20);
	}
	DrivetrainL.move(0);
	DrivetrainR.move(0);
}

void turn_left(float angle){
	float old_rotation = imu.get_rotation() - angle;
	while(imu.get_rotation() >= old_rotation){
		DrivetrainL.move(-50);
		DrivetrainR.move(50);
		pros::delay(20);
	}
	DrivetrainL.move(0);
	DrivetrainR.move(0);
}

void turn_right(float angle){
	float old_rotation = imu.get_rotation() + angle;
	while(imu.get_rotation() <= old_rotation){
		DrivetrainL.move(50);
		DrivetrainR.move(-50);
		pros::delay(20);
	}
	DrivetrainL.move(0);
	DrivetrainR.move(0);
}

void move_intake(void*) {
	while (true) {
		if (in_autonomous) {
			if (intake_control == 1) {
				Intake.move(127);
			}
			else if (intake_control == -1) {
				Intake.move(-127);
			}
			else { Intake.move(0);
			}
		}
		pros::delay(20);
	}
}

void move_storage(void*) {
	while (true) {
		if (in_autonomous) {
			if (storage_control == 1) {
				Storage.move(127);
			}
			else if (storage_control == -1) {
				Storage.move(-127);
			}
			else {
				Storage.move(0);
			}
		}
		pros::delay(20);
	}
}


void initialize() {
	imu.reset();
	while(imu.is_calibrating()){
		pros::delay(20);
	}
}

void disabled() {}

void competition_initialize() {}
pros::Task storage_task(move_storage, nullptr);
pros::Task intake_task(move_intake, nullptr);

void autonomous() {
	in_autonomous = true;
	if(autonomous_mode == 1){
		storage_control = 1;
		intake_control = 1;
		move_forward(39);
		storage_control = 0;
		intake_control = 0;
		turn_left(30);
		move_backward(38);
		turn_right(12);
		move_forward(16);
		storage_control = -1;
		intake_control = 1;
	}
	else if(autonomous_mode == -1){
		storage_control = 1;
		intake_control = 1;
		move_forward(39);
		storage_control = 0;
		intake_control = 0;
		turn_right(30);
		move_backward(38);
		turn_left(12);
		move_forward(16);
		storage_control = -1;
		intake_control = 1;
	}
	else if(autonomous_mode == 0){
		move_forward(1);
	}

}
void redrawscreen(){
	pros::screen::erase();
	if(driving_mode == 1){
			pros::screen::print(pros::E_TEXT_MEDIUM, 1, "Driving mode: arcade");
		}
		else if(driving_mode == 0){
			pros::screen::print(pros::E_TEXT_MEDIUM, 1, "Driving mode: tank");
		}
		if(autonomous_mode == 0){
			pros::screen::print(pros::E_TEXT_MEDIUM, 0, "Autonomous mode: none");
		}
		else if(autonomous_mode == 1){
			pros::screen::print(pros::E_TEXT_MEDIUM, 0, "Autonomous mode: right");
		}
		else if(autonomous_mode == -1){
			pros::screen::print(pros::E_TEXT_MEDIUM, 0, "Autonomous mode: left");
		}
	}
void opcontrol(){
	in_autonomous = false;
	DrivetrainL.set_gearing(pros::E_MOTOR_GEARSET_36);
	DrivetrainR.set_gearing(pros::E_MOTOR_GEARSET_36);
	Intake.set_gearing(pros::E_MOTOR_GEARSET_36);
	Storage.set_gearing(pros::E_MOTOR_GEARSET_36);
	DrivetrainL.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	DrivetrainR.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	Intake.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	Storage.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	redrawscreen();
	while(true){
		if(driving_mode == 0){
			DrivetrainL.move(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) * DRIVING_SPEED / 127);
			DrivetrainR.move(master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) * DRIVING_SPEED / 127);
		}
		else if(driving_mode == 1){
			int dir = master.get_analog(ANALOG_LEFT_Y);
			int turn = master.get_analog(ANALOG_RIGHT_X);
			DrivetrainL.move(dir + turn);
			DrivetrainR.move(dir - turn);
		}
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
			Intake.move(127);
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
			Intake.move(-127);
		} else {
			Intake.move(0);
		}
		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
			Storage.move(127);
		} else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
			Storage.move(-127);
		} else {
			Storage.move(0);
		} 
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) == 1){
			Arm.set_value(false);
		}else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) == 1){
			Arm.set_value(true);
		}
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT) == 1){
			DRIVING_SPEED = 127;
		} 
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT) == 1){
			DRIVING_SPEED = 50;
		}
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X) == 1 && master.get_digital(pros::E_CONTROLLER_DIGITAL_B) == 1 && master.get_digital(pros::E_CONTROLLER_DIGITAL_A) == 0 && master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) == 0){
			autonomous();
		}
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A) == 1){
			if(autonomous_mode == 1){
				autonomous_mode = -1;		
			}
			else if(autonomous_mode == -1){
				autonomous_mode = 0;	
			}
			else if(autonomous_mode == 0){
				autonomous_mode = 1;
			}
			redrawscreen();
			pros::delay(1000);
		}

		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_B) == 1){
			if(driving_mode == 0){
				driving_mode = 1;
			}
			else if(driving_mode == 1){
				driving_mode = 0;
			}
			redrawscreen();
			pros::delay(1000);
		}
		pros::delay(20);
	}
}

