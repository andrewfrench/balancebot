#include <Encoder.h>
#include <DualMC33926MotorShield.h>

class Motors {
  public:
    Motors(int, int, int, int);
    void manualUpdate(int, int);
    void stopRobot(void);
  private:
    Encoder * leftEncoder;
    Encoder * rightEncoder;
    DualMC33926MotorShield motorShield;
};

Motors::Motors(int lp0, int lp1, int rp0, int rp1) {
  *leftEncoder = Encoder(lp0, lp1); // Encoder(18, 19);
  *rightEncoder = Encoder(rp0, rp1); // Encoder(2, 3);

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
