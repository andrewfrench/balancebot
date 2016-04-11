#include <Encoder.h>
#include <DualMC33926MotorShield.h>

#define COUNTS_PER_REV  3200

class Motors {
  public:
    Motors(int, int, int, int);
    void updateSpeeds(int, int);
    void fullSpeed(void);
    void stopRobot(void);
    float getLeftEncoderVelocity(void);
    float getRightEncoderVelocity(void);
    int getLeftEncoderCount(void);
    int getRightEncoderCount(void);
  private:
    long lastCountLeft = 0;
    long lastCountRight = 0;
    long currentTimeLeft = 0;
    long currentTimeRight = 0;
    long lastCountTimeLeft = 0;
    long lastCountTimeRight = 0;
    Encoder * leftEncoder;
    Encoder * rightEncoder;
    DualMC33926MotorShield motorShield;
};

Motors::Motors(int lp0, int lp1, int rp0, int rp1) {
  leftEncoder = new Encoder(lp0, lp1); // Encoder(18, 19);
  rightEncoder = new Encoder(rp0, rp1); // Encoder(2, 3);

  lastCountLeft = 0;
  lastCountRight = 0;
  lastCountTimeLeft = micros();
  lastCountTimeRight = micros();

  motorShield.init();
  motorShield.setM1Speed(0);
  motorShield.setM2Speed(0);

  Serial.println("Motors initialized.");
}

void Motors::updateSpeeds(int left, int right) {
  // motorShield.setM1Speed(left);
  // motorShield.setM2Speed(-1 * right);

  motorShield.setSpeeds(left, -1 * right);
}

void Motors::stopRobot() {
  motorShield.setM1Speed(0);
  motorShield.setM2Speed(0);
}

int Motors::getLeftEncoderCount() {
  return leftEncoder->read();
}

int Motors::getRightEncoderCount() {
  return rightEncoder->read();
}

float Motors::getLeftEncoderVelocity() {
  // Get current reading time [usec]
  currentTimeLeft = micros();

  // Get current encoder count
  long leftCount = leftEncoder->read();

  // Get the difference in encoder counts since last reading
  long countDiffLeft = leftCount - lastCountLeft;

  // Replace last count with current count
  lastCountLeft = leftCount;

  // Find the number of revolutions since last reading
  float revsLeft = (1.0 * countDiffLeft) / (1.0 * COUNTS_PER_REV);

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
  currentTimeRight = micros();

  // Get current encoder count
  long rightCount = rightEncoder->read();

  // Get the difference in encoder counts since last reading
  long countDiffRight = rightCount - lastCountRight;

  // Replace last count with current count
  lastCountRight = rightCount;

  // Find the number of revolutions since last reading
  float revsRight = (1.0 * countDiffRight) / (1.0 * COUNTS_PER_REV);

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

void Motors::fullSpeed() {
  // motorShield.setSpeeds(400, 400);
  motorShield.setM1Speed(-200);
  motorShield.setM2Speed(200);
}
