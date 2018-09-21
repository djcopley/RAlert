#include <SPI.h>
#include "RF24.h"

/*** RaLert Detector ***/

#define VERSION_MAJOR 0
#define VERSION_MINOR 1

// Pin definitions
#define PIR_SENSOR_PIN 2

/****************** User Config *****************/
/***      Set this radio as number 1, 2       ***/
short radioNumber = 1;
byte addresses[][6] = {"1Node", "2Node", "3Node"};  // 2Node = W Corridor : 3Node = E Corridor
/************************************************/

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7, 8);

/*
 * Node1 - Master
 *   |- Node2 - Slave
 *   |- Node3 - Slave
 */

int movementDetected()
{
    return digitalRead(PIR_SENSOR_PIN);
}

void setup()
{
    // Enable serial
    Serial.begin(115200);

    // Set pinMode
    pinMode(PIR_SENSOR_PIN, INPUT);

#ifdef _DEBUG
    // Print Status
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
    radio.openWritingPipe(addresses[radioNumber]);
    radio.openReadingPipe(radioNumber, addresses[0]);
}

void loop()
{

    byte packet = radioNumber << 1;

    /*
     * byte packet 7 6 5 4 3 2 1 0
     *            |_________|___|_|
     *               Unused
     *                     Node ID
     *                         Movement
     */

    if (movementDetected()) {

        packet += 1; // Set motion detected to true

#ifdef _DEBUG
        Serial.println(F("Movement detected"));
        Serial.println(F("Sending packet"));
#endif

        bool success;
        do {
            success = radio.write(&packet, sizeof(byte));
        } while (!success);

#ifdef _DEBUG
        Serial.println(F("Packet Sent"));
#endif

        while (movementDetected()) {} // Hold until movement goes back low
    }
}
