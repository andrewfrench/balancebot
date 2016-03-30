#include <NewPing.h>

class Ultrasonic {
  public:
    Ultrasonic(int, int, int);
    long getDistance(void);
    bool obstacleDetected(void);
  private:
    int triggerPin;
    int echoPin;
    int obstacleThreshold;
    volatile long duration;
    long cm;
    NewPing * sonar;
};

Ultrasonic::Ultrasonic(int triggerPin, int echoPin, int obsThresh) {
  *sonar = NewPing(echoPin, triggerPin, 200);

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set obstacle threshold as passed into constructor
  obstacleThreshold = obsThresh;
}

long Ultrasonic::getDistance() {
  // Send a trigger pulse
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Wait for an echo
  duration = pulseIn(echoPin, HIGH);

  cm = (duration / 2) / 29.1;

  return cm;
}

bool Ultrasonic::obstacleDetected() {
  return (getDistance() <= obstacleThreshold);
}
