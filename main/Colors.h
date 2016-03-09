#include <Wire.h>
#include <SparkFun_APDS9960.h>

/*
Define color detection thresholds
These should be floats for easy comparison with sensor output.
*/
const float FINISH_LINE_THRESHOLD = 400.0;
const float RED_THRESHOLD = 300.0;
const float GREEN_THRESHOLD = 300.0;

// This will need to change to another interrupt pin
#define APDS9960_INT  2

// Define constants
#define PROX_INT_HIGH   0
#define PROX_INT_LOW    0

uint16_t ambient_light;
uint16_t red_light;
uint16_t green_light;
uint16_t blue_light;
uint8_t proximity_data;
SparkFun_APDS9960 apds = SparkFun_APDS9960();

void colors_init() {
  // Report status serially
  Serial.println("Initializing color sensor.");

  // Set LED as output
  // pinMode(APDS9960_INT, INPUT);

  // Initialize variables
  ambient_light = 0;
  red_light = 0;
  green_light = 0;
  blue_light = 0;
  proximity_data = 0;

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    // Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }

  // Start running the APDS-9960 light sensor (no interrupts)
  if ( apds.enableLightSensor(false) ) {
    // Serial.println(F("Light sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during light sensor init!"));
  }

  delay(500);

  Serial.println("Color sensor initialized.");
}

// Read the red light
float colors_getRed() {
  apds.readAmbientLight(ambient_light);
  apds.readRedLight(red_light);

  return (1.0 * red_light) / (1.0 * ambient_light);
}

// Read the green light
float colors_getGreen() {
  apds.readAmbientLight(ambient_light);
  apds.readGreenLight(green_light);

  return (1.0 * green_light) / (1.0 * ambient_light);
}

// Read the blue light
float colors_getBlue() {
  apds.readAmbientLight(ambient_light);
  apds.readBlueLight(blue_light);

  return (1.0 * blue_light) / (1.0 * ambient_light);
}

// Read the ambient light
int colors_getAmbient() {
    apds.readAmbientLight(ambient_light);

    return ambient_light;
}

bool colors_finishLineDetected() {
  return (colors_getBlue() > FINISH_LINE_THRESHOLD);
}
