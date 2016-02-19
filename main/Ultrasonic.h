#include <NewPing.h>

NewPing sonar(12, 11, 200);

int triggerPin = 11;
int echoPin = 12;
int LEDPin = 8;
volatile long duration;
long cm;

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
  pinMode(LEDPin, OUTPUT);

  // Begin serial communications
  Serial.println("Ultrasonic initialization complete.");
}

long ultrasonic_getDistance() {
  cm = (duration / 2) / 29.1;

  return cm;
}

ISR(TIMER1_COMPA_vect) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH);
}
