#include "Communication.h"
#include "Ultrasonic.h"
#include "Motors.h"
#include "Colors.h"

/*

main.ino

Ensure that all required libraries are installed on your development system.
Required libraries:
  Motors:
    - Encoder.h
    - DualMC33926MotorShield.h
  Colors:
    - SparkFun_APDS9960.h
  Communication:
    - RF24.h
  Ultrasonic:
    - NewPing.h

This is the top-level control file.  Specific input/output handling should
take place in the file that handles that hardware component.  Functions are
defined in included header files (e.g., Motors.h) and are called using their
respective prefixes (e.g., motors_init() to initialize the motor control code).
*/

void setup() {
  /*
  Initialize communication services - both Serial communications
  at 115200 baud for debugging and wireless communication for remote control.
  This should be done first to ensure communication is available for debugging
  the rest of the program.
  */
  comm_init();

  /*
  Initialize motors
  */
  motors_init();

  /*
  Initialize color sensor
  */
  colors_init();

  /*
  Initialize ultrasonic distance sensor
  */
  ultrasonic_init();

  /*
  The function will ensure the robot sits idle while waiting for the "go"
  command from the base station.  Once the command is received, the function
  will return and begin executing the rest of the program.
  */
  comm_waitForGo();
}

void loop() {
  /*
  General program flow:
    - Detect obstacles using ultrasonic distance sensor
      - If obstacle detected, stop robot
    - Detect blue finish line using color sensor
      - If finish line detected, stop robot
    - Maintain straight trajectory using input from color sensor
      and motor set points
  */

  if(ultrasonic_obstacleDetected()) {
    motors_stopRobot();
  }

  if(colors_finishLineDetected()) {
    motors_stopRobot();
  }

  // Detect red
    // Angle right

  // Detect green
    // Angle left

  // use motors_updateSpeed(float duty_cycle_left, float duty_cycle_right) to update motor speed
}
