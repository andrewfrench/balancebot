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
    double getPValue(void);
    double getDValue(void);
    double getIValue(void);
    double getTimeDiff(void);
    void tick(void);
  private:
    void findTarget(void);
    void getEValue(void);
    long currentTime;
    long lastTime;
    double timeDiff;
    long e_value_prev = 0;
    long e_value = 0;
    double d_value = 0;
    double d_value_prev = 0;
    double i_value_prev = 0;
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

  findTarget();

  lastTime = micros();
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

void Accelerometer::tick() {
  getEValue();
}

double Accelerometer::getTimeDiff() {
  return timeDiff;
}

double Accelerometer::getPValue() {
  // P[k] = E[k]

  return e_value;
}

double Accelerometer::getDValue() {
  // D[k] = (E[k] - E[k - 1]) / Ts
  d_value_prev = d_value;
  d_value = 0.75 * (0.01 * ((1.0 * e_value) - (1.0 * e_value_prev)) / (1.0 * timeDiff)) + (0.99 * d_value_prev);
  return d_value;
}

double Accelerometer::getIValue() {
  // I[k] = I[k - 1] + E[k] * Ts

  long i_value = 1.5 * (0.02 * (i_value_prev + (e_value * timeDiff))) + (0.98 * i_value_prev);

  if(i_value > 30000) {
    i_value = 30000;
  } else if (i_value < -30000) {
    i_value = -30000;
  }

  i_value_prev = i_value;

  return i_value;
}

void Accelerometer::getEValue() {
  e_value_prev = e_value;
  long reading = getX1();
  e_value = reading - target;

  currentTime = micros();
  timeDiff = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;
}

int Accelerometer::getX1() {
  int value = analogRead(xPin1);

  if(value > 2 * target) {
    value = 2 * target;
  }

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
