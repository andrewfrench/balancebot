/*
   Dec 2014 - TMRh20 - Updated
   Derived from examples by J. Coliz <maniacbug@ymail.com>
*/
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

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9,53);
/**********************************************************/
                                                                           // Topology
byte addresses[][6] = {"abcde","edcba"};              // Radio pipe addresses for the 2 nodes to communicate.

// Role management: Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.
typedef enum { role_ping_out = 1, role_pong_back } role_e;                 // The various roles supported by this sketch
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};  // The debug-friendly names of those roles
role_e role = role_pong_back;                                              // The role of the current running sketch

byte counter = 1;                                                          // A single byte to keep track of the data being sent back and forth


void comm_init(){

  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted_CallResponse"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

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

  radio.writeAckPayload(1,&counter,1);          // Pre-load an ack-paylod into the FIFO buffer for pipe 1
  //radio.printDetails();
}

byte gotByte;

byte comm_waitForGo() {
  Serial.println("Waiting for go command (27)...");
  while(1) {
    /****************** Pong Back Role ***************************/

    if ( role == role_pong_back ) {
      byte pipeNo;     // Declare variables for the pipe and the byte received
      while( radio.available(&pipeNo)){              // Read all available payloads
        radio.read( &gotByte, 1 );
        // Since this is a call-response. Respond directly with an ack payload.
        //gotByte += 1;                                // Ack payloads are much more efficient than switching to transmit mode to respond to a call
        radio.writeAckPayload(pipeNo,&gotByte, 1 );  // This can be commented out to send empty payloads.
        Serial.print(F("Loaded next response "));
        Serial.println(gotByte);
      }

      if(gotByte == 27) {
        Serial.println("gotByte is 27");
        return gotByte;
      }
    }

    /****************** Change Roles ***************************/

    if(role == role_ping_out) {
      role = role_pong_back;
      Serial.println("Pong back role");
    }
  }
}
