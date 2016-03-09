#include <Encoder.h>
#include <DualMC33926MotorShield.h>

/*---------------------Controls----------------------*/

 // Instantiate motor driver and encoder objects
DualMC33926MotorShield md;
Encoder motorEncoderLeft(18, 19); // Corresponds to m1
Encoder motorEncoderRight(2, 3); // Corresponds to m2

void motors_init() {

  md.init();
  md.setM1Speed(0);
  md.setM2Speed(0);

}

void motors_manualUpdateSpeed(int left, int right) {
  right = -right;
  md.setM1Speed(left);
  md.setM2Speed(right);
}

void motors_stopRobot() {
  md.setM1Speed(0);
  md.setM2Speed(0);
}

// Declare variables for time functions
const long waitTime = 1000;
long timeLast[] = {0, 0};
long timeDelta;

// Declare variables for position functions
long lastPosition[] = {0, 0};
long posDiff;

// Declare variables for PI controller functions
const float gainProportional[] = {0.5797, .5086};
const float gainIntegral[] = {13.9785, 9.3345};
const float maxAngularVelocity = 20.9;
float desiredAngularVelocity[] = {2,2};
float angleDifference;
float measuredAngularVelocity;
float errorProportional;
float errorIntegral[] = {0, 0};
float errorIntegralLimit = 2;
float percentPower;
float previousPercentPower[] = {0,0};
float percentDelta = 10;

int currentMotor = 1;
char outputString[64];


long motors_getTimeDelta(int motorIndex) {
  long timeNow = micros(); // Get current time
  timeDelta = timeNow - timeLast[motorIndex-1]; // Calculate time since last measurement/write
  timeLast[motorIndex-1] = timeNow;
  return timeDelta;
}

long motors_readEncoder(int motorIndex) {
  long pos;
  long posDifference;
  if (motorIndex == 1) {
    pos = motorEncoderLeft.read();
  } else if (motorIndex == 2) {
    pos = motorEncoderRight.read();
  } else {

    return 0;
  }
  posDifference = pos - lastPosition[motorIndex-1];
  lastPosition[motorIndex-1] = pos;
  return posDifference;
}

float motors_calculateRadsPerSec(long positionDifferenceIn, long timeDifferenceIn) {
  return positionDifferenceIn * .0019635 / (timeDifferenceIn / 1000000.0); // <-- Dividing by 10^6 to convert timeDelta (us) to seconds
}

int motors_matchSpeed(int sp1, int sp2) {
  long pd1 = motors_readEncoder(1);
  long pd2 = motors_readEncoder(2);

  long td1 = motors_getTimeDelta(1);
  long td2 = motors_getTimeDelta(2);

  float rps1 = motors_calculateRadsPerSec(pd1, td1);
  float rps2 = motors_calculateRadsPerSec(pd2, td2);

  if(rps1 > rps2) {
    sp2 += 3;
  } else if(rps1 < rps2) {
    sp2 -= 3;
  }

  Serial.println();
  Serial.println(sp1);
  Serial.println(sp2);
  Serial.println(rps1);
  Serial.println(rps2);

  return sp2;
}

float motors_controlsBlockPI(float measuredIn, float desiredIn, long timeDeltaIn, int motorIndex) {
  // Calculate instantaneous and accumulated error
  errorProportional = desiredIn - measuredIn;
  errorIntegral[motorIndex-1] = errorIntegral[motorIndex-1] + errorProportional * (timeDeltaIn / 1000000.0); // errorIntegral += error * 1 ms
  //Prevent integrator windup
  if (errorIntegral[motorIndex-1] > errorIntegralLimit) {
    errorIntegral[motorIndex-1] = errorIntegralLimit;
  }
  // Calculate percent power to be applied, [-100..100]
  percentPower = (errorProportional * gainProportional[motorIndex-1] + errorIntegral[motorIndex-1]*gainIntegral[motorIndex-1]) * 100/maxAngularVelocity;

  if (percentPower > 100) {
    percentPower = 100;
  } else if (percentPower < 0) {
    percentPower = 0;
  }
  return percentPower;
}

void motors_updateSpeed(float dutyCycle, int motorIndex) {
  int motorSpeed = int(400L * dutyCycle / 100);
  if (motorIndex == 1) {
    md.setM1Speed(motorSpeed); // scaling is from +/- 1024. 1024 is cast as long to prevent overflow of int type during intermediary multiplication step
  } else if (motorIndex == 2) {
    md.setM2Speed(-motorSpeed);
  } else {

  }
}

void motors_changeCurrentMotor() {
  if (currentMotor == 1) {
    currentMotor = 2;
  } else if (currentMotor == 2) {
    currentMotor = 1;
  } else {

  }
}

float motors_accelLimit(float percentPowerIn, float previousPercentPowerIn) {
  if (percentPowerIn > previousPercentPowerIn) {
    if (percentPowerIn - previousPercentPowerIn > percentDelta) {
      return previousPercentPowerIn + percentDelta;
    } else return percentPowerIn;
  } else {
    if (previousPercentPowerIn - percentPowerIn > percentDelta) {
      return previousPercentPowerIn - percentDelta;
    } else return percentPowerIn;
  }
}

void motors_setMotorVelocity(int motorIn, float velocityIn) {
  desiredAngularVelocity[motorIn - 1] = velocityIn;
  timeDelta = motors_getTimeDelta(motorIn);
  posDiff = motors_readEncoder(motorIn);
  measuredAngularVelocity = motors_calculateRadsPerSec(posDiff, timeDelta);
  float calculatedPercentPower = motors_controlsBlockPI(measuredAngularVelocity, desiredAngularVelocity[motorIn], timeDelta, motorIn);
  percentPower = motors_accelLimit(calculatedPercentPower, previousPercentPower[motorIn - 1]);
  previousPercentPower[motorIn-1] = percentPower;
  motors_updateSpeed(percentPower, motorIn);
}
