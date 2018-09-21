#include <SPI.h>
#include "RF24.h"

/*** RaLert Receiver ***/

#define VERSION_MAJOR 0
#define VERSION_MINOR 1

// Receiver station is always Node1
short radioNumber = 0;
byte addresses[][6] = {"1Node", "2Node", "3Node"};  // 2Node = W Corridor : 3Node = E Corridor

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7, 8);

/*
 * Node1 - Master
 *   |- Node2 () - Slave
 *   |- Node3 () - Slave
 */


void setup()
{
    // Enable serial
    Serial.begin(115200);

#ifdef _DEBUG
    radioNumber == 0 ? Serial.println(F("Initializing RaLert - Receiver...")) : Serial.println(
            F("Initializing RaLert - Detector..."));
#endif

    // Initialize
    radio.begin();

    // Set PA Level - RF24_PA_MAX or RF24_PA_LOW
    radio.setPALevel(RF24_PA_MAX);

    // Set data rate - lower increases range
    radio.setDataRate(RF24_250KBPS);

    // Open a writing pipe and reading pipes for each node
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);
    radio.openReadingPipe(2, addresses[2]);

    // Start listening for data
    radio.startListening();
}

void loop()
{
    if (radio.available()) {

#ifdef _DEBUG
        Serial.println(F("Packet Received"));
#endif

        byte rxPacket;

        /*
         * byte rxPacket 7 6 5 4 3 2 1 0
         *              |_________|___|_|
         *                 Unused
         *                       Node ID
         *                           Movement
         */

        while (radio.available()) {
            radio.read(&rxPacket, sizeof(byte));
        }

        Serial.write(rxPacket);
    }
}
