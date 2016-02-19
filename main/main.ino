#include "Communication.h"
#include "Ultrasonic.h"
#include "Motors.h"
#include "Colors.h"

/*

main.ino

The top-level control file.

Ensure that all required libraries are installed on your development machine.
We will probably need to migrate to the Arduino Mega, because we have a lot of pin
collisions as is.

Use the current function definitions, and Andrew will create a class structure
next week.

 */

void setup() {
  // Initialize serial communications
  Serial.begin(115200);

  // Initialize motors
  motors_init();
  
  // Initialize wireless communications
  comm_connect();

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
  float * colors = colors_getColors();
  if(colors[2] < 2) {
    motors_stopMotors();
  }

  // Detect object ultrasonically
  long cm = ultrasonic_getDistance();
  if(cm < 5) {
    motors_stopMotors();
  }
    // Stop robot
}
