#include <NewPing.h>

class Ultrasonic {
  public:
    Ultrasonic(int, int, int);
    int getDistance(void);
    bool obstacleDetected(void);
  private:
    int triggerPin;
    int echoPin;
    int obstacleThreshold;
    volatile long duration;
    int cm;
    NewPing * sonar;
};

Ultrasonic::Ultrasonic(int triggerPin, int echoPin, int obsThresh) {
  *sonar = NewPing(triggerPin, echoPin);

  // Set obstacle threshold as passed into constructor
  obstacleThreshold = obsThresh;

  Serial.println("Ultrasonic configured.");
}

int Ultrasonic::getDistance() {
  // Send a trigger pulse and record cm
  cm = sonar->ping() / US_ROUNDTRIP_CM;

  return cm;
}

bool Ultrasonic::obstacleDetected() {
  return (getDistance() <= obstacleThreshold);
}
