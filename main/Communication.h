#include <SPI.h>
#include "RF24.h"

// Set radioNumber to 1, the base station will be 0.
bool radioNumber = 1;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/
                                                                           // Topology
byte addresses[][6] = {"abcde","edcba"};              // Radio pipe addresses for the 2 nodes to communicate.

void comm_connect(){
  // Setup serial communication for debugging
  Serial.begin(115200);

  // Setup and configure radio
  radio.begin();

  radio.enableAckPayload();                     // Allow optional ack payloads
  radio.enableDynamicPayloads();                // Ack payloads are dynamic payloads

  if(radioNumber){
    radio.openWritingPipe(addresses[1]);        // Both radios listen on the same pipes by default, but opposite addresses
    radio.openReadingPipe(1,addresses[0]);      // Open a reading pipe on address 0, pipe 1
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  radio.startListening();                       // Start listening

  // radio.writeAckPayload(1,&counter,1);          // Pre-load an ack-paylod into the FIFO buffer for pipe 1
  //radio.printDetails();
}

byte comm_waitForGo() {
  Serial.println("Waiting for go command (27)...");
  while(1) {

    byte pipeNo, gotByte;                          // Declare variables for the pipe and the byte received
    while( radio.available(&pipeNo) ){              // Read all available payloads
      radio.read( &gotByte, 1 );
      // Since this is a call-response. Respond directly with an ack payload.
                                                   // Ack payloads are much more efficient than switching to transmit mode to respond to a call
      radio.writeAckPayload(pipeNo,&gotByte, 1 );  // This can be commented out to send empty payloads.

      if(gotByte == 27) {
        Serial.println("Go command (27) received.");
        return gotByte;
      }
    }
  }
}
