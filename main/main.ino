// #include "Communication.h"
// #include "Ultrasonic.h"
// #include "Colors.h"
#include "Motors.h"
#include "Accelerometer.h"

/*

main.ino

The top-level control file.

 */

// Define radio configuration values
#define COMMUNICATION_CE_PIN           9
#define COMMUNICATION_CS_PIN           53

// Define ultrasonic sensor configuration values
#define ULTRASONIC_OBSTACLE_THRESHOLD  8
#define ULTRASONIC_TRIGGER_PIN         5
#define ULTRASONIC_ECHO_PIN            6

// Define color sensor configuration values
#define COLORS_FINISH_LINE_THRESHOLD   400.0

// Define motor control configuration values
#define LEFT_ENC_PIN_0                 18
#define LEFT_ENC_PIN_1                 19
#define RIGHT_ENC_PIN_0                20
#define RIGHT_ENC_PIN_1                21

// Define accelerometer configuration values
#define ACCEL_X_X1_PIN                 A2
#define ACCEL_X_X4_PIN                 A4
#define ACCEL_Z_X1_PIN                 A3
#define ACCEL_Z_X4_PIN                 A5
#define ACCEL_HP_PIN                   2

Accelerometer * accelerometer;
Motors * motors;

void setup() {
  Serial.begin(115200);
  // Communication communication(COMMUNICATION_CE_PIN, COMMUNICATION_CS_PIN);
  // Ultrasonic ultrasonic(ULTRASONIC_OBSTACLE_THRESHOLD, ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN);
  // Colors colors(COLORS_FINISH_LINE_THRESHOLD);
  motors = new Motors(LEFT_ENC_PIN_0, LEFT_ENC_PIN_1, RIGHT_ENC_PIN_0, RIGHT_ENC_PIN_1);
  accelerometer = new Accelerometer(ACCEL_X_X1_PIN, ACCEL_X_X4_PIN, ACCEL_Z_X1_PIN, ACCEL_Z_X4_PIN, ACCEL_HP_PIN);
}

void loop() {
  Serial.print("Tu,");
  Serial.print(micros());
  Serial.print(",Xa,");
  Serial.print(accelerometer->getX1());
  Serial.print(",Za,");
  Serial.print(accelerometer->getZ1());
  Serial.print(",Ev,");
  Serial.print(motors->getLeftEncoderVelocity());
}
