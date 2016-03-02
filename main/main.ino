#include "Communication.h"
#include "Ultrasonic.h"
#include "Motors.h"
#include "Colors.h"
#include <Encoder.h>
#include <DualMC33926MotorShield.h>


/*

main.ino

Ensure that all required libraries are installed on your development system.
Required libraries:
  Motors:
    - Encoder.h
    - DualMC33926MotorShield.h
  Colors:
    - SparkFun_APDS9960.h
  Communication:
    - RF24.h
  Ultrasonic:
    - NewPing.h

This is the top-level control file.  Specific input/output handling should
take place in the file that handles that hardware component.  Functions are
defined in included header files (e.g., Motors.h) and are called using their
respective prefixes (e.g., motors_init() to initialize the motor control code).
*/

/*---------------------Controls----------------------*/

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
const float gainProportional[] = {0.5797, .5086};
const float gainIntegral[] = {13.9785, 9.3345};
const float maxAngularVelocity = 20.9;
const float desiredAngularVelocity[] = {2,2};
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

void setMotorVelocity(motorIn, velocityIn) {
  desiredAngularVelocity[motorIn] = velocityIn;
  timeDelta = getTimeDelta(motorIn);
  posDiff = readEncoder(motorIn);
  measuredAngularVelocity = calculateRadsPerSec(posDiff, timeDelta);
  percentPower = controlBlockPI(measuredAngularVelocity, desiredAngularVelocity[motorIn], timeDelta, motorIn);
  updateSpeed(percentPower, motorIn);
}

void setup() {
<<<<<<< HEAD
  /*
  Initialize communication services - both Serial communications
  at 115200 baud for debugging and wireless communication for remote control.
  This should be done first to ensure communication is available for debugging
  the rest of the program.
  */
  comm_init();

  /*
  Initialize motors
  */
  motors_init();

  /*
  Initialize color sensor
  */
  colors_init();

  /*
  Initialize ultrasonic distance sensor
  */
  ultrasonic_init();

  /*
  The function will ensure the robot sits idle while waiting for the "go"
  command from the base station.  Once the command is received, the function
  will return and begin executing the rest of the program.
  */
=======
  md.init();
  // Serial.begin(115200);
  // Initialize wireless communications
  comm_connect();

  // Initialize motors
  // motors_init();

  // Initialize color sensor
  // colors_init();

  // Initialize ultrasonic distance sensor
  // ultrasonic_init();

  // Wait for "go" signal
>>>>>>> 84cd6d5b82ba3aff72fed0609bd7168c07a4cca7
  comm_waitForGo();
}

void loop() {
  /*
  General program flow:
    - Detect obstacles using ultrasonic distance sensor
      - If obstacle detected, stop robot
    - Detect blue finish line using color sensor
      - If finish line detected, stop robot
    - Maintain straight trajectory using input from color sensor
      and motor set points
  */

  if(ultrasonic_obstacleDetected()) {
    motors_stopRobot();
  }

  if(colors_finishLineDetected()) {
    motors_stopRobot();
  }

  // Detect red
    // Angle right

  // Detect green
    // Angle left

<<<<<<< HEAD
  // use motors_updateSpeed(float duty_cycle_left, float duty_cycle_right) to update motor speed
=======
  // Detect blue
  // if(colors_getBlue() < 2.0) {
  //   motors_stopMotors();
  // }

  // Detect object ultrasonically
  // long cm = ultrasonic_getDistance();
  // if(cm < 5) {
  //   // Stop robot
  //   motors_stopMotors();
  // }

/*---------------------Controls----------------------*/

  setMotorVelocity(currentMotor, desiredAngularVelocity[currentMotor]);
  changeCurrentMotor();
  
>>>>>>> 84cd6d5b82ba3aff72fed0609bd7168c07a4cca7
}
