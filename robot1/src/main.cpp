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
}


void disabled() {}


void competition_initialize() {}


void autonomous() {}


pros::Controller master(pros::E_CONTROLLER_MASTER);

// temp port numbers
pros::MotorGroup drive_left({1, 2, 3, 4, 5}, pros::v5::MotorGears::green); 
pros::MotorGroup drive_right({6, 7, 8, 9, 10}, pros::v5::MotorGears::green);


void opcontrol() {
	// Deadzone to catch stick drift
	const int DEADZONE = 10;

	while (true) {
		int forward = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

		if (std::abs(forward) < DEADZONE) forward = 0;
		if (std::abs(turn) < DEADZONE) turn = 0;

		int left_power = forward + turn;
		int right_power = forward - turn;


		drive_left.move(left_power);
		drive_right.move(right_power);

		pros::delay(20);
	}
}