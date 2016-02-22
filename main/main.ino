#include "Communication.h"
#include "Ultrasonic.h"
#include "Motors.h"
#include "Colors.h"

/*

main.ino

The top-level control file.

Ensure that all required libraries are installed on your development machine.

 */

void setup() {
  // Initialize wireless communications
  comm_connect();

  // Initialize motors
  motors_init();

  // Wait for "go" signal
  while(1) {
    byte b = comm_receive();
    Serial.println(b);
  }
}

void loop() {
  // use motors_updateSpeed(float duty_cycle_left, float duty_cycle_right) to update motor speed

  // Detect red
    // Angle right

  // Detect green
    // Angle left

  // Detect blue
  if(colors_getBlue() < 2.0) {
    motors_stopMotors();
  }

  // Detect object ultrasonically
  long cm = ultrasonic_getDistance();
  if(cm < 5) {
    // Stop robot
    motors_stopMotors();
  }
}
