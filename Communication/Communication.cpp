#include <SPI.h>
#include "RF24.h"

bool radioNumber = 0;

class Communication {
public:
  bool connect(void);
  byte receive(void);
private:
  static bool radioNumber = 0;
  static byte robotAddress[6] = "ROBOT";
  static byte contrAddress[6] = "CONTR";
  RF24 radio(7, 8);
};

bool Communication::connect() {
  /*
    Returns true if connection was successful
    Returns false if connection was unsuccessful
  */

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
}

byte Communication::receive() {
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
