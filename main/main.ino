#include "Communication.h"
#include "Ultrasonic.h"
#include "Motors.h"
#include "Colors.h"
#include <Encoder.h>
#include <DualMC33926MotorShield.h>


/*

main.ino

The top-level control file.

Ensure that all required libraries are installed on your development machine.

 */

void setup() {
  // Serial.begin(115200);
  // Initialize wireless communications
  comm_init();

  // Initialize motors
  motors_init();

  // Initialize color sensor
  colors_init();

  // Initialize ultrasonic distance sensor
  ultrasonic_init();

  // Wait for "go" signal
  comm_waitForGo();

  Serial.println("Executing rest of code...");
}

void loop() {
  if(ultrasonic_obstacleDetected()) {
    motors_stopRobot();
  }

  if(colors_finishLineDetected()) {
    motors_stopRobot();
  }

/*---------------------Controls----------------------*/

  motors_setMotorVelocity(currentMotor, desiredAngularVelocity[currentMotor]);
  motors_changeCurrentMotor();

}
