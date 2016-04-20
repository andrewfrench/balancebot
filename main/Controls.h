class Controls {
  public:
    Controls();
    float getPValue(void);
    float getIValue(void);
    float getDValue(void);
    float getTimeDiff(void);
    void tick(void);
    void updateAngle(int);
    float getAngle(void);
  private:
    void updateEValue(void);
    long currentTime;
    long lastTime;
    long timeDiff;
    long actualAngle;
    long targetAngle;
    long prevI;
    long currI;
    float prevP;
    float currE;
    float prevE;
};

Controls::Controls() {
  // Initialize values
  currentTime = micros() / 1000;
  lastTime = 0;
  timeDiff = 0;

  prevI = 0;
  currI = 0;
  prevE = 0;
  prevP = 0;
  currE = 0;

  actualAngle = 0;
  targetAngle = 0;
}

float Controls::getPValue() {
  return currE;
}

float Controls::getIValue() {
  prevI = currI;
  currI = (currE * timeDiff) + prevI;
  return currI;
}

float Controls::getDValue() {
  return (currE - prevE) / timeDiff;
}

void Controls::updateEValue() {
  prevE = currE;
  currE = actualAngle - targetAngle;
}

float Controls::getTimeDiff() {
  return timeDiff;
}

void Controls::updateAngle(int accelerometerValue) {
  lastTime = currentTime;
  currentTime = micros() / 1000;

  timeDiff = currentTime - lastTime;

  actualAngle += accelerometerValue * timeDiff;

  updateEValue();
}

float Controls::getAngle() {
  return actualAngle;
}
