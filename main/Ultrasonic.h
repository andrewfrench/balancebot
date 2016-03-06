#include <NewPing.h>

/*
Define obstacle detection threshold
*/
#define OBSTACLE_THRESHOLD 10

int triggerPin = 5;
int echoPin = 4;
volatile long duration;
long cm;

NewPing sonar(echoPin, triggerPin, 200);

void ultrasonic_init() {
  Serial.println("Initializing ultrasonic sensor.");

  // Set up input and output pins
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Begin serial communications
  Serial.println("Ultrasonic sensor initialized.");
}

long ultrasonic_getDistance() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  cm = (duration / 2) / 29.1;

  Serial.print("Reading distance: ");
  Serial.print(cm);
  Serial.println("cm");

  return cm;
}

bool ultrasonic_obstacleDetected() {
  return (ultrasonic_getDistance() < OBSTACLE_THRESHOLD);
}
