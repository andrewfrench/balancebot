#include <Wire.h>
#include <SparkFun_APDS9960.h>

/*
Define color detection thresholds
These should be floats for easy comparison with sensor output.
*/
const float FINISH_LINE_THRESHOLD = 400.0;

// This will need to change to another interrupt pin
#define APDS9960_INT  2

// Define constants
#define PROX_INT_HIGH   0
#define PROX_INT_LOW    0

class Colors {
  public:
    Colors(void);
    int getRed(void);
    int getGreen(void);
    int getBlue(void);
    int getAmbient(void);
    bool finishLineDetected(void);
  private:
    uint16_t ambient;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    SparkFun_APDS9960 sensor;
    float finish_threshold;
};

Colors::Colors() {
  sensor = SparkFun_APDS9960();

  ambient = 0;
  red = 0;
  green = 0;
  blue = 0;

  finish_threshold = FINISH_LINE_THRESHOLD;

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( sensor.init() ) {
    // Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }

  // Start running the APDS-9960 light sensor (no interrupts)
  if ( sensor.enableLightSensor(false) ) {
    // Serial.println(F("Light sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during light sensor init!"));
  }

  delay(500);

}

int Colors::getRed() {
  sensor.readRedLight(red);

  return red;
}

int Colors::getGreen() {
  sensor.readGreenLight(green);

  return green;
}

int Colors::getBlue() {
  sensor.readBlueLight(blue);

  return blue;
}

int Colors::getAmbient() {
  sensor.readAmbientLight(ambient);

  return ambient;
}

bool Colors::finishLineDetected() {
  return (getBlue() > FINISH_LINE_THRESHOLD);
}
