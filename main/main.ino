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

// Ultrasonic * ultrasonic;
Accelerometer * accelerometer;
// Colors * colors;
Motors * motors;

const double k_p = 0.01;
const double k_d = 30;
const double k_i = 0.025;

void setup() {
  Serial.begin(115200);
  Serial.println("Running robot.");
  // communication = new Communication(COMMUNICATION_CE_PIN, COMMUNICATION_CS_PIN);
  // ultrasonic = new Ultrasonic(ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN, ULTRASONIC_OBSTACLE_THRESHOLD);
  // colors = new Colors(COLORS_FINISH_LINE_THRESHOLD);
  motors = new Motors(LEFT_ENC_PIN_0, LEFT_ENC_PIN_1, RIGHT_ENC_PIN_0, RIGHT_ENC_PIN_1);
  accelerometer = new Accelerometer(ACCEL_X_X1_PIN, ACCEL_X_X4_PIN, ACCEL_Z_X1_PIN, ACCEL_Z_X4_PIN, ACCEL_HP_PIN);
}

void loop() {
  accelerometer->tick();
  double p_value = accelerometer->getPValue();
  double i_value = accelerometer->getIValue();
  double d_value = accelerometer->getDValue();
  double t_diff  = accelerometer->getTimeDiff();

  double control_value = (k_p * p_value) + (k_i * i_value) + (k_d * d_value);
  if(control_value > 400) {
    control_value = 400;
  } else if(control_value < -400) {
    control_value = -400;
  }

  Serial.print("V: ");
  Serial.print(control_value);

  Serial.print(", dT: ");
  Serial.print(t_diff);

  Serial.print(", P: ");
  Serial.print(p_value);

  Serial.print(", I: ");
  Serial.print(i_value);

  Serial.print(", D: ");
  Serial.print(d_value);

  Serial.print("\n");

  motors->updateSpeeds((int) control_value, (int) control_value);

}
