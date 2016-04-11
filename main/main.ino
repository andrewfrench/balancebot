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
// These changed from A2, A4, A3, A5
#define ACCEL_X_X1_PIN                 A8
#define ACCEL_X_X4_PIN                 A10
#define ACCEL_Z_X1_PIN                 A9
#define ACCEL_Z_X4_PIN                 A11
#define ACCEL_HP_PIN                   2

// Ultrasonic * ultrasonic;
Accelerometer * accelerometer;
// Colors * colors;
Motors * motors;

const double k_p = 0.02;
const double k_d = 40;
const double k_i = 0.32;

void setup() {
  Serial.begin(115200);
  Serial.println("Running robot.");
  Serial.println("Waiting five seconds...");
  delay(5000);
  Serial.println("Beginning.");
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

  int p_contrib = k_p * p_value;
  int i_contrib = k_i * i_value;
  int d_contrib = k_d * d_value;

  float l_count = motors->getLeftEncoderCount();
  float r_count = -1 * motors->getRightEncoderCount();

  int control_value = p_contrib + i_contrib + d_contrib;
  if(control_value > 400) {
    control_value = 400;
  } else if(control_value < -400) {
    control_value = -400;
  }

  Serial.print("V: ");
  Serial.print(control_value);

  Serial.print(",   \tdT: ");
  Serial.print(t_diff);

  Serial.print(",   \tP: ");
  Serial.print(p_contrib);

  Serial.print(",   \tI: ");
  Serial.print(i_contrib);

  Serial.print(",   \tD: ");
  Serial.print(d_contrib);

  Serial.print(",   \tL: ");
  Serial.print(l_count);

  Serial.print(",   \tR: ");
  Serial.print(r_count);

  Serial.print("\n");

  motors->updateSpeeds(control_value, control_value);

  // Serial.print("A: ");
  // Serial.print(accelerometer->getX1());
  // Serial.print("\n");
}
