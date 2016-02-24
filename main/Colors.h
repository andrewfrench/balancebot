#include <Wire.h>
#include <SparkFun_APDS9960.h>

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
SparkFun_APDS9960 apds;

void colors_init() {
  // Report status serially
  Serial.println("Initializing color sensor...");

  // Set LED as output
  pinMode(APDS9960_INT, INPUT);

  // Initialize sensor
  apds = SparkFun_APDS9960();

  // Initialize variables
  ambient_light = 0;
  red_light = 0;
  green_light = 0;
  blue_light = 0;
  proximity_data = 0;

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }

  // Start running the APDS-9960 light sensor (no interrupts)
  if ( apds.enableLightSensor(false) ) {
    Serial.println(F("Light sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during light sensor init!"));
  }

  delay(500);

  // Adjust the Proximity sensor gain
  if ( !apds.setProximityGain(PGAIN_2X) ) {
    Serial.println(F("Something went wrong trying to set PGAIN"));
  }

  // Set proximity interrupt thresholds
  if ( !apds.setProximityIntLowThreshold(PROX_INT_LOW) ) {
    Serial.println(F("Error writing low threshold"));
  }
  if ( !apds.setProximityIntHighThreshold(PROX_INT_HIGH) ) {
    Serial.println(F("Error writing high threshold"));
  }

  // Start running the APDS-9960 proximity sensor (interrupts)
  if ( apds.enableProximitySensor(true) ) {
    Serial.println(F("Proximity sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during sensor init!"));
  }

  Serial.println("Color sensor initialization process complete.");
}

float colors_getRed() {
  apds.readAmbientLight(ambient_light);
  apds.readRedLight(red_light);

  Serial.print("Reading red: ");
  Serial.println(red_light);

  return (ambient_light * 1.0) / (red_light * 1.0);
}

float colors_getGreen() {
  apds.readAmbientLight(ambient_light);
  apds.readGreenLight(green_light);

  Serial.print("Reading green: ");
  Serial.println(green_light);

  return (ambient_light * 1.0) / (green_light * 1.0);
}

float colors_getBlue() {
  apds.readAmbientLight(ambient_light);
  apds.readBlueLight(blue_light);

  Serial.print("Reading blue: ");
  Serial.println(blue_light);

  return (ambient_light * 1.0) / (blue_light * 1.0);
}
