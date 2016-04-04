#include <Encoder.h>
#include <DualMC33926MotorShield.h>

#define COUNTS_PER_REV  3200

class Motors {
  public:
    Motors(int, int, int, int);
    void manualUpdate(int, int);
    void stopRobot(void);
    float getLeftEncoderVelocity(void);
    float getRightEncoderVelocity(void);
  private:
    long lastCountLeft;
    long lastCountRight;
    long lastCountTimeLeft;
    long lastCountTimeRight;
    Encoder * leftEncoder;
    Encoder * rightEncoder;
    DualMC33926MotorShield motorShield;
};

Motors::Motors(int lp0, int lp1, int rp0, int rp1) {
  *leftEncoder = Encoder(lp0, lp1); // Encoder(18, 19);
  *rightEncoder = Encoder(rp0, rp1); // Encoder(2, 3);

  lastCountLeft = 0;
  lastCountRight = 0;
  lastCountTimeLeft = micros();
  lastCountTimeRight = micros();

  motorShield.init();
  motorShield.setM1Speed(0);
  motorShield.setM2Speed(0);
}

void Motors::manualUpdate(int left, int right) {
  motorShield.setM1Speed(left);
  motorShield.setM2Speed(right);
}

void Motors::stopRobot() {
  motorShield.setM1Speed(0);
  motorShield.setM2Speed(0);
}

float Motors::getLeftEncoderVelocity() {
  // Get current reading time [usec]
  long currentTimeLeft = micros();

  // Get current encoder count
  long leftCount = leftEncoder->read();

  // Get the difference in encoder counts since last reading
  long countDiffLeft = leftCount - lastCountLeft;

  // Find the number of revolutions since last reading
  float revsLeft = countDiffLeft / COUNTS_PER_REV;

  // Convert revolutions to radians
  float radsLeft = revsLeft * 2 * 3.1415;

  // Find the time difference between readings
  long timeDiffLeft = currentTimeLeft - lastCountTimeLeft;

  // Replace last time reading with current time reading
  lastCountTimeLeft = currentTimeLeft;

  // Convert radians to radians/sec, mult by 1e6 to convert from usec to sec
  float radsPerSecLeft = (radsLeft / timeDiffLeft) * 1000000;

  return radsPerSecLeft;
}

float Motors::getRightEncoderVelocity() {
  // Get current reading time [usec]
  long currentTimeRight = micros();

  // Get current encoder count
  long rightCount = rightEncoder->read();

  // Get the difference in encoder counts since last reading
  long countDiffRight = rightCount - lastCountRight;

  // Find the number of revolutions since last reading
  float revsRight = countDiffRight / COUNTS_PER_REV;

  // Convert revolutions to radians
  float radsRight = revsRight * 2 * 3.1415;

  // Find the time difference between readings
  long timeDiffRight = currentTimeRight - lastCountTimeRight;

  // Replace last time reading with current time reading
  lastCountTimeRight = currentTimeRight;

  // Convert radians to radians/sec, mult by 1e6 to convert from usec to sec
  float radsPerSecRight = (radsRight / timeDiffRight) * 1000000;

  return radsPerSecRight;
}
