#include <Encoder.h>
#include <DualMC33926MotorShield.h>

DualMC33926MotorShield md;
Encoder left_encoder(3, 2);
Encoder right_encoder(5, 4);

void motors_init() {
  Serial.println("Initializing motors");
  md.init();
}

void motors_updateSpeed(float duty_cycle_left, float duty_cycle_right) {
  md.setM1Speed(duty_cycle_left);
  md.setM2Speed(duty_cycle_right);
}

void motors_stopRobot() {
  md.setM1Speed(0);
  md.setM2Speed(0);
}
