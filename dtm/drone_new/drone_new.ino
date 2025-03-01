#include <SPI.h>
#include <Wire.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal.h>  // Include the LiquidCrystal library for parallel LCD

RF24 radio(9, 10);  // CE, CSN pins for nRF24L01
byte addresses[][6] = {"1Ctrl", "2Ctrl"};  // Different pipes for control and OTA update

bool isUpdateMode = false;  // Flag to track if firmware update is in progress
int ledPin1 = 2;  // LED to simulate motor forward/backward movement
int ledPin2 = 3;  // Another LED to simulate motor forward/backward movement

// LCD pin connections using available pins
LiquidCrystal lcd(A0, A1, 4, 5, 6, 7); // RS=A0, E=A1, D4=4, D5=5, D6=6, D7=7

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.setChannel(0);  // Listening on control channel
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);  // Read from update pipe
  radio.startListening();

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  // Initialize the LCD
  lcd.begin(16, 2);  // Set the LCD to 16x2 display
  lcd.print("Drone Ready");

  delay(1000);  // Wait for the LCD to initialize
}

void loop() {
  if (radio.available()) {
    char receivedData[32] = "";
    radio.read(&receivedData, sizeof(receivedData));

    // Detect the OTA update signal to begin the update process
    if (receivedData[0] == 'U') {  // "U" indicates update mode signal
      if (receivedData[1] == 'S') {  // "S" for OTA start
        Serial.println("OTA update signal detected! Entering update mode...");
        isUpdateMode = true;
        startFirmwareUpdate();
      }
    }
  }

  // If not in update mode, process control commands to move the drone
  if (!isUpdateMode) {
    processControlCommands();
  }
}

// Control function to move the drone based on controller commands
void processControlCommands() {
  char controlData[32];
  
  // Simulate control data being received
  if (radio.available()) {
    radio.read(&controlData, sizeof(controlData));

    if (controlData[0] == 'F') {  // Forward
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, LOW);
      lcd.clear();
      lcd.print("Moving Forward");
    } else if (controlData[0] == 'B') {  // Backward
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, HIGH);
      lcd.clear();
      lcd.print("Moving Backward");
    } else if (controlData[0] == 'L') {  // Left
      lcd.clear();
      lcd.print("Turning Left");
    } else if (controlData[0] == 'R') {  // Right
      lcd.clear();
      lcd.print("Turning Right");
    }
  }
}

// Firmware update function to accept and process OTA firmware
void startFirmwareUpdate() {
  char firmwareData[32];
  
  lcd.clear();
  lcd.print("OTA Update Mode");

  while (isUpdateMode) {
    if (radio.available()) {
      radio.read(&firmwareData, sizeof(firmwareData));
      Serial.print("Received firmware chunk: ");
      Serial.println(firmwareData);
      
      // Display firmware update status on the LCD
      lcd.setCursor(0, 1);
      lcd.print("Updating...");

      // Simulate processing the firmware chunk
      processFirmware(firmwareData);

      delay(500);
    }
  }
}

void processFirmware(char *data) {
  // Simulate processing the firmware data
  Serial.print("Processing firmware chunk: ");
  Serial.println(data);
  // Normally, firmware would be written to memory or applied here
}
