#include "Communication.h"
#include "Ultrasonic.h"
#include "Motors.h"
#include "Colors.h"

/*

main.ino

The top-level control file.

 */

// Define global variables here
int speedLeft = 110;
int speedRight = 110;
int maxLeft = 110;
int maxRight = 100;

float increment = 0.03;

float defaultLeft = 5.0;
float defaultRight = 5.0;

void setup() {
  Serial.begin(115200);
  // Initialize wireless communications
  comm_init();

  // Initialize color sensor
  colors_init();

  // Initialize ultrasonic distance sensor
  ultrasonic_init();

  // Wait for "go" signal
  // comm_waitForGo();

  // Initialize motors
  motors_init();
}

void loop() {
  // Set the speed as calculated by previous loop
  motors_manualUpdateSpeed(speedLeft, speedRight);

  // Grab color sensor outputs for math below
  float red = colors_getRed();
  float green = colors_getGreen();
  float blue = colors_getBlue();
  int ambient_light = colors_getAmbient();

  // Find color sensor ratios
  float red_ratio = red / green;
  float green_ratio = green / red;
  float blue_red = blue / red;
  float blue_green = blue / green;

  // Determine if stop conditions have been met
  if(blue_red > 1.0 && blue_green > 1.0 && ambient_light > 50) {
    motors_stopRobot();
    while(1);
  }

  // Determine if an object is detected ultrasonically
  if(ultrasonic_obstacleDetected()) {
    motors_stopRobot();
    while(1);
  }

  // Update the speed of the motors
  speedRight = motors_matchSpeed(speedLeft, speedRight);
}
