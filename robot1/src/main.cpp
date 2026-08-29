#include "main.h"

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


void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);

	double external_ratio = 48.0 / 36.0; // 48:36 external gear ratio
}


void disabled() {}


void competition_initialize() {}


void autonomous() {}


pros::Controller master(pros::E_CONTROLLER_MASTER);

// temp port numbers
pros::MotorGroup drive_fl({1, -2}, pros::v5::MotorGears::green); 
pros::MotorGroup drive_fr({-3, 4}, pros::v5::MotorGears::green);
pros::MotorGroup drive_bl({5, -6}, pros::v5::MotorGears::green);
pros::MotorGroup drive_br({-7, 8}, pros::v5::MotorGears::green);


void opcontrol() {
	// Deadzone to catch stick drift
	const int DEADZONE = 10;

	while (true) {
		int forward = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int strafe = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);
		int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

		if (std::abs(forward) < DEADZONE) forward = 0;
		if (std::abs(strafe) < DEADZONE) strafe = 0;
		if (std::abs(turn) < DEADZONE) turn = 0;

		int fl_power = forward + strafe + turn;
		int bl_power = forward - strafe + turn;
		int fr_power = forward - strafe - turn;
		int br_power = forward + strafe - turn;

		drive_fl.move(fl_power);
		drive_bl.move(bl_power);
		drive_fr.move(fr_power);
		drive_br.move(br_power);

		pros::delay(20);
	}
}