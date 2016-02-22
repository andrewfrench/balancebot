#include <SPI.h>
#include "RF24.h"

RF24 radio(7, 8);
const bool radioNumber = 0;
const byte robotAddress[6] = "ROBOT";
const byte contrAddress[6] = "CONTR";

void comm_connect() {

  // Handle initialization of serial comms here
  Serial.begin(115200);
  Serial.println("Initializing wireless communication...");

  // Begin the radio service
  radio.begin();

  // Enable ack payloads
  radio.enableAckPayload();

  // Ack payloads are dynamic payloads
  radio.enableDynamicPayloads();

  // Open writing pipe
  radio.openWritingPipe(contrAddress);

  // Open reading pipe
  radio.openReadingPipe(1, robotAddress);

  // Start listening
  radio.startListening();

  Serial.println("Wireless communication initialized.");
}

byte comm_receive() {
  /*
    Listens for wireless communication and returns the byte
  */

  // Create byte to be returned
  byte received;

  // Create pipe number byte
  byte pipeNo;

  while(radio.available(&pipeNo)) {
    radio.read(&received, 1);

    // Return the received byte to the caller
    return received;
  }
}
