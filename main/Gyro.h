// X in A0
// Z in A1
// HP in pin 2

class Gyro {
  public:
    Gyro(int, int, int, int, int);
    int getX1(void);
    int getZ1(void);
    int getX4(void);
    int getZ4(void);
    void findTarget(void);
  private:
    int xPin1;
    int zPin1;
    int xPin4;
    int zPin4;
    int hpPin;
    long target = 0;
};

Gyro::Gyro(int xp1, int xp4, int zp1, int zp4, int hpp) {
  xPin1 = xp1;
  zPin1 = zp1;
  xPin4 = xp4;
  zPin4 = zp4;

  hpPin = hpp;

  // Set up pin modes
  pinMode(hpPin, OUTPUT);

  // High-pass filter reset
  digitalWrite(hpPin, LOW);
  digitalWrite(hpPin, HIGH);
  delay(500);
  digitalWrite(hpPin, LOW);

  // Allow the sensor voltage to rise to steady state
  delay(1000);
}

void Gyro::findTarget() {
  int loops = 1000;
  for(int i = 0; i < loops; i++) {
    target += analogRead(xPin1);
    Serial.print("T: ");
    Serial.print(target / i);
    Serial.print("\n");
  }

  target /= loops;

  Serial.print("T: ");
  Serial.print(target);
  Serial.print("\n");
}

int Gyro::getX1() {
  int value = analogRead(xPin1) - target;

  return value;
}

int Gyro::getZ1() {
  return analogRead(zPin1);
}

int Gyro::getX4() {
  return analogRead(xPin4);
}

int Gyro::getZ4() {
  return analogRead(zPin4);
}
