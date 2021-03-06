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
    void updateTarget(long);
    long getTarget(void);
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
    float currP;
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
  currP = 0;
  currE = 0;

  actualAngle = 0;
  targetAngle = 0;
}

void Controls::updateTarget(long target) {
  targetAngle = target;
}

long Controls::getTarget(void) {
  return targetAngle;
}

float Controls::getPValue() {
  return currE;
}

float Controls::getIValue() {
  prevI = currI;
  currI = 1.02 * ((currE * timeDiff) + prevI);
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

void Controls::updateAngle(int gyroValue) {

  // if(abs(gyroValue) < 5) {
  //   gyroValue = 0;
  // }

  lastTime = currentTime;
  currentTime = micros() / 1000;

  timeDiff = currentTime - lastTime;

  actualAngle += gyroValue * timeDiff;

  updateEValue();
}

float Controls::getAngle() {
  return actualAngle;
}
