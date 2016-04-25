// #include "Communication.h"
// #include "Ultrasonic.h"
// #include "Colors.h"
#include "Motors.h"
#include "Gyro.h"
#include "Controls.h"

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

// Define gyro configuration values
// These changed from A2, A4, A3, A5
#define GYRO_X_X1_PIN                 A8
#define GYRO_X_X4_PIN                 A10
#define GYRO_Z_X1_PIN                 A9
#define GYRO_Z_X4_PIN                 A11
#define GYRO_HP_PIN                   2

// Ultrasonic * ultrasonic;
Gyro * gyro;
// Colors * colors;
Motors * motors;
Controls * controls;

const float k_p = 0.0066;
const float k_d = 0.06;
const float k_i = 0.000015;

void setup() {
  Serial.begin(115200);
  Serial.println("Running robot.");
  Serial.println("Waiting five seconds...");
  delay(3000);
  Serial.println("Beginning.");
  // communication = new Communication(COMMUNICATION_CE_PIN, COMMUNICATION_CS_PIN);
  // ultrasonic = new Ultrasonic(ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN, ULTRASONIC_OBSTACLE_THRESHOLD);
  // colors = new Colors(COLORS_FINISH_LINE_THRESHOLD);
  motors = new Motors(LEFT_ENC_PIN_0, LEFT_ENC_PIN_1, RIGHT_ENC_PIN_0, RIGHT_ENC_PIN_1);
  gyro = new Gyro(GYRO_X_X1_PIN, GYRO_X_X4_PIN, GYRO_Z_X1_PIN, GYRO_Z_X4_PIN, GYRO_HP_PIN);
  controls = new Controls();

  gyro->findTarget();
}

void loop() {
  controls->updateAngle(gyro->getX1());
  float p_value = controls->getPValue();
  float i_value = controls->getIValue();
  float d_value = controls->getDValue();
  float t_diff  = controls->getTimeDiff();

  int p_contrib = k_p * p_value;
  int i_contrib = k_i * i_value;
  int d_contrib = k_d * d_value;

  int control_value = p_contrib + i_contrib + d_contrib;

  float l_velocity = motors->getLeftEncoderVelocity();
  float r_velocity = -1 * motors->getRightEncoderVelocity();

  if(control_value > 400) {
    control_value = 400;
  } else if(control_value < -400) {
    control_value = -400;
  }

  motors->updateSpeeds(control_value, control_value);

  // if(micros() > 20000000) {
  //   controls->updateTarget(10000);
  // }

  Serial.print("Time [ms]: ");
  Serial.print(micros() / 1000);


  Serial.print(",   \tX: ");
  Serial.print(gyro->getX1());


  Serial.print(",   \tA: ");
  Serial.print(controls->getTarget());

  Serial.print(",   \tdT [ms]: ");
  Serial.print(t_diff);

  Serial.print(",   \tA: ");
  Serial.print(controls->getAngle());

  Serial.print(",   \tP: ");
  Serial.print(p_contrib);

  Serial.print(",   \tI: ");
  Serial.print(i_contrib);

  Serial.print(",   \tD: ");
  Serial.print(d_contrib);

  Serial.print(",   \tControl: ");
  Serial.print(control_value);

  //
  // Serial.print(",   \tdL [rad/s]: ");
  // Serial.print(l_velocity);
  //
  // Serial.print(",   \tdR [rad/s]: ");
  // Serial.print(r_velocity);

  Serial.print("\n");
}
