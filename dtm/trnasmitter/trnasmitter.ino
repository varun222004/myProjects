#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);  // CE, CSN pins for nRF24L01
byte address[] = "1OTA";

const int totalPackets = 10;  // Define the total number of packets you wish to send

void setup() {
    Serial.begin(9600);
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_HIGH);
    delay(100);
}

void initiateOTA() {
    char startSignal[] = "U";  // 'U' for OTA Update
    radio.write(&startSignal, sizeof(startSignal));  // Send OTA start signal
    delay(1000);  // Short delay before sending firmware
}

void sendFirmwareData() {
    char firmwarePacket[32] = "SampleFirmwareData";  // Populate with sample data
    for (int i = 0; i < totalPackets; i++) {
        radio.write(&firmwarePacket, sizeof(firmwarePacket));
        delay(50);  // Adjust delay for data handling time
        Serial.print("Sending firmware packet ");
        Serial.println(i + 1);
    }
}

void loop() {
    initiateOTA();  // Start the OTA process
    sendFirmwareData();  // Send the firmware in chunks
}
