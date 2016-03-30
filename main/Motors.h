#include <Encoder.h>
#include <DualMC33926MotorShield.h>

class Motors {
  public:
    Motors(void);
    void manualUpdate(int, int);
    void stopRobot(void);
  private:
    Encoder * leftEncoder;
    Encoder * rightEncoder;
    DualMC33926MotorShield motorShield;
};

Motors::Motors() {
  *leftEncoder = Encoder(18, 19);
  *rightEncoder = Encoder(2, 3);

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
