// X in A0
// Z in A1
// HP in pin 2

class Accelerometer {
  public:
    Accelerometer(int, int, int, int, int);
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

Accelerometer::Accelerometer(int xp1, int xp4, int zp1, int zp4, int hpp) {
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

void Accelerometer::findTarget() {
  // target = getX1();
  for(int i = 0; i < 1000; i++) {
    target += analogRead(xPin1);
    Serial.print("T: ");
    Serial.print(target / i);
    Serial.print("\n");
  }

  target /= 1000;

  Serial.print("T: ");
  Serial.print(target);
  Serial.print("\n");
}

int Accelerometer::getX1() {
  int value = analogRead(xPin1) - target;

  return value;
}

int Accelerometer::getZ1() {
  return analogRead(zPin1);
}

int Accelerometer::getX4() {
  return analogRead(xPin4);
}

int Accelerometer::getZ4() {
  return analogRead(zPin4);
}
