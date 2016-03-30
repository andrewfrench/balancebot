/**
 * Example for efficient call-response using ack-payloads
 *
 * This example continues to make use of all the normal functionality of the radios including
 * the auto-ack and auto-retry features, but allows ack-payloads to be written optionlly as well.
 * This allows very fast call-response communication, with the responding radio never having to
 * switch out of Primary Receiver mode to send back a payload, but having the option to switch to
 * primary transmitter if wanting to initiate communication instead of respond to a commmunication.
 */

#include <SPI.h>
#include "RF24.h"

class Communication {
  public:
    Communication(int, int);
    byte waitForGo(void);
    bool send(char *);
    char * read(void);
  private:
    RF24 * radio;
    byte * writeAddress;
    byte * readAddress;
    bool radioNumber;
    byte counter;
    byte gotByte;
};

typedef enum {
  role_ping_out = 1,
  role_pong_back
} role_e;

Communication::Communication(int ce_pin, int cs_pin) {
  // TODO: Write code to send and receive general data on command.
  Serial.begin(115200);

  *radio = RF24(ce_pin, cs_pin); // RF24(9, 53);

  radioNumber = true;

  role_e role = role_pong_back;

  byte counter = 1;

  radio->begin();

  radio->enableAckPayload();                     // Allow optional ack payloads
  radio->enableDynamicPayloads();                // Ack payloads are dynamic payloads

  if(radioNumber){
    radio->openWritingPipe(writeAddress);        // Both radios listen on the same pipes by default, but opposite addresses
    radio->openReadingPipe(1,readAddress);      // Open a reading pipe on address 0, pipe 1
  }else{
    radio->openWritingPipe(writeAddress);
    radio->openReadingPipe(1,readAddress);
  }
  radio->startListening();                       // Start listening

  radio->writeAckPayload(1,&counter,1);          // Pre-load an ack-paylod into the FIFO buffer for pipe 1
}

bool Communication::send(char * data) {
  // Send away telemetry data
}

char * Communication::read() {
  // Receive controls data
}

byte Communication::waitForGo() {
  Serial.println("Waiting for go command (27)...");
  while(1) {
    byte pipeNo;     // Declare variables for the pipe and the byte received
    while( radio->available(&pipeNo)){              // Read all available payloads
      radio->read( &gotByte, 1 );
      // Since this is a call-response. Respond directly with an ack payload.
      //gotByte += 1;                                // Ack payloads are much more efficient than switching to transmit mode to respond to a call
      radio->writeAckPayload(pipeNo,&gotByte, 1 );  // This can be commented out to send empty payloads.
      Serial.print(F("Loaded next response "));
      Serial.println(gotByte);
    }

    if(gotByte == 27) {
      Serial.println("gotByte is 27");
      return gotByte;
    }
  }
}
