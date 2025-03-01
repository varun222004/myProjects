#include <SPI.h>
#include <RF24.h>
#include <Servo.h>  // Include the Servo library

// Servo motor objects
Servo servo1; // Servo for pitch control
Servo servo2; // Servo for yaw control
Servo servo3; // Servo for roll control
Servo servo4; // Servo for throttle or altitude control

// nRF24L01+ module (CE, CSN)
RF24 radio(9, 10); 
const byte address[6] = "00001";  // Unique address

// Variables for received commands and firmware storage
const int chunkSize = 32;  // Size of firmware chunks
byte firmwareBuffer[chunkSize];  // Buffer for firmware chunks
bool isFirmwareUpdate = false;  // Flag to check if it's a firmware update

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);

  // Initialize servo motors
  servo1.attach(3);  // Attach servo1 to pin 3
  servo2.attach(5);  // Attach servo2 to pin 5
  servo3.attach(6);  // Attach servo3 to pin 6
  servo4.attach(9);  // Attach servo4 to pin 9

  // Initialize nRF24L01+ radio
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();

  Serial.println("Drone ready.");
}

void loop() {
  if (radio.available()) {
    radio.read(firmwareBuffer, chunkSize);  // Read data

    // Check if it's a firmware chunk
    if (firmwareBuffer[0] == 'F' && firmwareBuffer[1] == 'W') {  
      // This indicates it's part of a firmware update (simple check based on the first two characters)
      isFirmwareUpdate = true;
      // Handle firmware update logic here (e.g., write to EEPROM or flash)
      Serial.println("Firmware update chunk received.");

      // Here we would store the firmware chunk in memory or an external device
      // For simplicity, just print out the chunk received:
      for (int i = 0; i < chunkSize; i++) {
        Serial.print(firmwareBuffer[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
      delay(100);  // Give some time to process the chunk
    }
    else {
      // Handle regular movement commands (Forward, Backward, Left, Right, Up, Down)
      if (isFirmwareUpdate) {
        Serial.println("Firmware update in progress, ignoring movement commands.");
        return;
      }

      String receivedCommand = String((char*)firmwareBuffer);
      Serial.print("Received command: ");
      Serial.println(receivedCommand);

      // Process movement commands
      if (receivedCommand == "FWD") {
        moveForward();
      } else if (receivedCommand == "BK") {
        moveBackward();
      } else if (receivedCommand == "LFT") {
        turnLeft();
      } else if (receivedCommand == "RGT") {
        turnRight();
      } else if (receivedCommand == "UP") {
        ascend();
      } else if (receivedCommand == "DN") {
        descend();
      }
    }
  }
}

// Functions to control the drone's movements with servos
void moveForward() {
  servo1.write(90);  // Set the pitch (forward)
  servo2.write(90);  // Set yaw
  servo3.write(45);  // Set roll (left tilt)
  servo4.write(90);  // Set throttle
}

void moveBackward() {
  servo1.write(180);  // Set the pitch (backward)
  servo2.write(90);   // Set yaw
  servo3.write(135);  // Set roll (right tilt)
  servo4.write(90);   // Set throttle
}

void turnLeft() {
  servo1.write(90);  // Set pitch (neutral)
  servo2.write(45);  // Turn yaw left
  servo3.write(90);  // Set roll (neutral)
  servo4.write(90);  // Set throttle
}

void turnRight() {
  servo1.write(90);  // Set pitch (neutral)
  servo2.write(135); // Turn yaw right
  servo3.write(90);  // Set roll (neutral)
  servo4.write(90);  // Set throttle
}

void ascend() {
  servo1.write(90);  // Set pitch (neutral)
  servo2.write(90);  // Set yaw (neutral)
  servo3.write(90);  // Set roll (neutral)
  servo4.write(180); // Set throttle (maximum for ascent)
}

void descend() {
  servo1.write(90);  // Set pitch (neutral)
  servo2.write(90);  // Set yaw (neutral)
  servo3.write(90);  // Set roll (neutral)
  servo4.write(0);   // Set throttle (minimum for descent)
}