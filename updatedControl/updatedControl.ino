#include <Encoder.h>
#include <DualMC33926MotorShield.h>

// Instantiate motor driver and encoder objects
DualMC33926MotorShield md;
Encoder motorEncoderLeft(18, 19); // Corresponds to m1
Encoder motorEncoderRight(21, 20); // Corresponds to m2

// Declare variables for time functions
const long waitTime = 1000;
long timeLast[] = {0, 0};
long timeDelta;

// Declare variables for position functions
long lastPosition[] = {0, 0};
long posDiff;

// Declare variables for PI controller functions
const float gainProportional[] = {0.5797, .059};
const float gainIntegral[] = {13.9785, 13.9785};
const float maxAngularVelocity = 20.7;
const float desiredAngularVelocity = 2;
float angleDifference;
float measuredAngularVelocity;
float errorProportional;
float errorIntegral[] = {0, 0};
float errorIntegralLimit = 2;
float percentPower;

int currentMotor = 1;
char outputString[64];


long getTimeDelta(int motorIndex) {
  long timeNow = micros(); // Get current time
  timeDelta = timeNow - timeLast[motorIndex-1]; // Calculate time since last measurement/write
  timeLast[motorIndex-1] = timeNow;
  return timeDelta;
}

long readEncoder(int motorIndex) {
  long pos;
  long posDifference;
  if (motorIndex == 1) {
    pos = motorEncoderLeft.read();
  } else if (motorIndex == 2) {
    pos = motorEncoderRight.read();
  } else {
    Serial.println("Error in read encoder: Invalid motor index. Pass 1 for the left motor, and 2 for the right motor.");
    return 0;
  }
  posDifference = pos - lastPosition[motorIndex-1];
  lastPosition[motorIndex-1] = pos;
  return posDifference;
}

float calculateRadsPerSec(long positionDifferenceIn, long timeDifferenceIn) {
  return positionDifferenceIn * .0019635 / (timeDifferenceIn / 1000000.0); // <-- Dividing by 10^6 to convert timeDelta (us) to seconds
}

float controlBlockPI(float measuredIn, float desiredIn, long timeDeltaIn, int motorIndex) {
  // Calculate instantaneous and accumulated error
  errorProportional = desiredIn - measuredIn;
  errorIntegral[motorIndex-1] = errorIntegral[motorIndex-1] + errorProportional * (timeDeltaIn / 1000000.0); // errorIntegral += error * 1 ms
  //Prevent integrator windup
  if (errorIntegral[motorIndex-1] > errorIntegralLimit) {
    errorIntegral[motorIndex-1] = errorIntegralLimit;
  }
  // Calculate percent power to be applied, [-100..100]
  percentPower = (errorProportional*gainProportional[motorIndex-1] + errorIntegral[motorIndex-1]*gainIntegral[motorIndex-1]) * 100/maxAngularVelocity;
  return percentPower;
}

void updateSpeed(float dutyCycle, int motorIndex) {
  int motorSpeed = int(1024L * dutyCycle / 100);
  if (motorIndex == 1) {
    md.setM1Speed(motorSpeed); // scaling is from +/- 1024. 1024 is cast as long to prevent overflow of int type during intermediary multiplication step
  } else if (motorIndex == 2) {
    md.setM2Speed(-motorSpeed);
  } else {
    Serial.println("Error in update speed: Invalid motor index. Pass 1 for the left motor, and 2 for the right motor.");
  }
}

void changeCurrentMotor() {
  if (currentMotor == 1) {
    currentMotor = 2;
  } else if (currentMotor == 2) {
    currentMotor = 1;
  } else {
    Serial.println("Invalid currentMotor index in changeCurrentMotor()");
  }
}

void setup() {
  md.init();
  Serial.begin(9600);
  Serial.println("Connected");
}

void loop() {

  timeDelta = getTimeDelta(currentMotor);
  posDiff = readEncoder(currentMotor);
  measuredAngularVelocity = calculateRadsPerSec(posDiff, timeDelta);
  percentPower = controlBlockPI(measuredAngularVelocity, desiredAngularVelocity, timeDelta, currentMotor);
  updateSpeed(percentPower, currentMotor);

  Serial.println(currentMotor);
  Serial.println(measuredAngularVelocity);

  // Print info
//  Serial.print("Motor ");
//  Serial.println(currentMotor);
//  Serial.print(": ");
//  Serial.print(percentPower*12/100);
//  Serial.print(" V, ");
//  Serial.print(micros()/1000); // Time in ms
//  Serial.print(" ms, ");
//  Serial.println(measuredAngularVelocity);
//  Serial.println(" rad/s");

  changeCurrentMotor();
}
