#include <NewPing.h>

/*
Define obstacle detection threshold
*/
#define OBSTACLE_THRESHOLD  10

int triggerPin = 5;
int echoPin = 3;
volatile long duration;
long cm;

NewPing sonar(echoPin, triggerPin, 200);

void ultrasonic_init() {
  Serial.println("Beginning ultrasonic initialization.");

  // Create timing interrupts
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A  = 255;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  sei();

  // Set up input and output pins
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Begin serial communications
  Serial.println("Ultrasonic initialization complete.");
}

long ultrasonic_getDistance() {
  cm = (duration / 2) / 29.1;

  Serial.print("Reading distance: ");
  Serial.print(cm);
  Serial.println("cm");

  return cm;
}

bool ultrasonic_obstacleDetected() {
  return (ultrasonic_getDistance() < OBSTACLE_THRESHOLD);
}

ISR(TIMER1_COMPA_vect) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH);
}
