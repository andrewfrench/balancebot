#include <NewPing.h>

// Define obstacle detection thresholds and pins
#define OBSTACLE_THRESHOLD  8
#define TRIGGER_PIN         5
#define ECHO_PIN            6

class Ultrasonic {
  public:
    Ultrasonic(void);
    long getDistance(void);
    bool obstacleDetected(void);
  private:
    int triggerPin;
    int echoPin;
    volatile long duration;
    long cm;
    NewPing * sonar;
};

Ultrasonic::Ultrasonic() {
  // Constructor

  triggerPin = TRIGGER_PIN;
  echoPin = ECHO_PIN;

  *sonar = NewPing(echoPin, triggerPin, 200);

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
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
  return (getDistance() <= OBSTACLE_THRESHOLD);
}
